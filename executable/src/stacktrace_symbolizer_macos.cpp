#include "stacktrace_symbolizer.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

namespace syan {

class StacktraceSymbolizerImpl final: public StacktraceSymbolizer {
public:
  StacktraceSymbolizerImpl(const std::string& binary_file_path,
                           const DumpFileHeader& dump_file_header)
      : binary_file_path(binary_file_path), dump_file_header(dump_file_header) {
    int stdin_pipe[2];
    if (pipe(stdin_pipe) < 0) {
      std::cerr << "Failed to open stdin pipe to stacktrace symbolizer process"
                << " errno=" << errno << " (" << strerror(errno) << ")\n";
      std::abort();
    }

    int stdout_pipe[2];
    if (pipe(stdout_pipe) < 0) {
      std::cerr << "Failed to open stdout pipe to stacktrace symbolizer process"
                << " errno=" << errno << " (" << strerror(errno) << ")\n";
      std::abort();
    }

    atos_pid = fork();
    if (atos_pid < 0) {
      std::cerr << "Failed to fork stacktrace symbolizer process"
                << " errno=" << errno << " (" << strerror(errno) << ")\n";
      std::abort();
    }
    if (atos_pid > 0) {  // parent is done.
      close(stdin_pipe[PIPE_READ]);
      close(stdout_pipe[PIPE_WRITE]);
      atos_stdin = stdin_pipe[PIPE_WRITE];
      atos_stdout = stdout_pipe[PIPE_READ];

      int flags = fcntl(atos_stdout, F_GETFL, 0);
      if (flags < 0) {
        std::cerr << "Failed to set atos stdout non-blocking."
                  << "fcntl(atos_stdout, F_GETFL, 0): errno=" << errno << " ("
                  << strerror(errno) << ")\n";
        std::abort();
      }
      flags |= O_NONBLOCK;
      int status = fcntl(atos_stdout, F_SETFL, flags);
      if (status < 0) {
        std::cerr << "Failed to set atos stdout non-blocking."
                  << "fcntl(atos_stdout, F_SETFL, flags): errno=" << errno
                  << " (" << strerror(errno) << ")\n";
        std::abort();
      }

      return;
    }

    if (dup2(stdin_pipe[PIPE_READ], STDIN_FILENO) < 0) {
      std::cerr << "Failed to pipe stacktrace symbolizer process's stdin"
                << " errno=" << errno << " (" << strerror(errno) << ")\n";
      std::abort();
    }

    if (dup2(stdout_pipe[PIPE_WRITE], STDOUT_FILENO) < 0) {
      std::cerr << "Failed to pipe stacktrace symbolizer process's stdout"
                << " errno=" << errno << " (" << strerror(errno) << ")\n";
      std::abort();
    }

    close(stdin_pipe[PIPE_READ]);
    close(stdin_pipe[PIPE_WRITE]);
    close(stdout_pipe[PIPE_READ]);
    close(stdout_pipe[PIPE_WRITE]);

    std::stringstream load_address_builder;
    load_address_builder << std::hex << std::setfill('0') << std::setw(16)
                         << dump_file_header.program_load_addr + 8;
    std::string load_address = load_address_builder.str();
    // TODO: Don't hardcode architecture here!
    const char* args[] = {"/usr/bin/atos",
                          "-arch",
                          "x86_64",
                          "-o",
                          binary_file_path.c_str(),
                          "-l",
                          load_address.c_str(),
                          nullptr};
    if (execv("/usr/bin/atos", (char* const*)args) != 0) {
      std::cerr << "Failed to execute stacktrace symbolizer process"
                << " errno=" << errno << " (" << strerror(errno) << ")\n";
      std::abort();
    }
  }

  ~StacktraceSymbolizerImpl() final {
    kill(atos_pid, SIGKILL);
    close(atos_stdin);
    close(atos_stdout);
  }

  void symbolize_stacktrace(RawBacktrace stack_trace, std::ostream& out) final {
    std::stringstream atos_command_builder;
    for (const auto& pc : stack_trace) {
      if (pc != 0) {
        atos_command_builder << "0x" << std::hex << std::setfill('0')
                             << std::setw(16) << pc << " ";
      }
    }
    atos_command_builder << '\n';
    std::string atos_command = atos_command_builder.str();
    size_t total_written = 0;
    while (total_written < atos_command.size()) {
      ssize_t written = write(atos_stdin, atos_command.data() + total_written,
                              atos_command.size() - total_written);
      if (written < 0) {
        std::cerr << "Failed to write input to stacktrace symbolizer"
                  << " errno=" << errno << " (" << strerror(errno) << ")\n";
        std::abort();
      }
      total_written += written;
    }

    int num_lines_expected = std::count_if(std::begin(stack_trace),
                                           std::end(stack_trace), [](auto pc) {
                                             return pc != 0;
                                           });

    int num_lines = 0;
    char ch;
    int num_consecutive_read_failures = 0;
    std::stringstream formatter;
    formatter << "\n\t\t";
    while (num_consecutive_read_failures < 20000) {
      errno = 0;
      auto read_status = read(atos_stdout, &ch, 1);
      if (read_status < 1) {
        if (errno == 0 || errno == EAGAIN) {
          num_consecutive_read_failures += 1;
          usleep(10);
          continue;
        }
        std::cerr << "Failed to read output of stacktrace symbolizer"
                  << " errno=" << errno << " (" << strerror(errno) << ")\n";
        std::abort();
      }
      num_consecutive_read_failures = 0;
      formatter << ch;
      if (ch == '\n') {
        num_lines += 1;
        if (num_lines == num_lines_expected) {
          break;
        }
        formatter << "\t\t";
      }
    }
    if (num_consecutive_read_failures == 20000) {
      out << "\n\t\tFailed to symbolize stacktrace.";
    } else {
      out << formatter.str();
    }
  }

private:
  int atos_pid;
  int atos_stdin;
  int atos_stdout;
  std::string binary_file_path;
  DumpFileHeader dump_file_header;
};

std::unique_ptr<StacktraceSymbolizer>
StacktraceSymbolizer::do_create(const std::string& binary_file_path,
                                const DumpFileHeader& dump_file_header) {
  return std::make_unique<StacktraceSymbolizerImpl>(binary_file_path,
                                                    dump_file_header);
}

}  // namespace syan
