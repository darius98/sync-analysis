#include "stacktrace_symbolizer.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <cstring>
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

    addr2line_pid = fork();
    if (addr2line_pid < 0) {
      std::cerr << "Failed to fork stacktrace symbolizer process"
                << " errno=" << errno << " (" << strerror(errno) << ")\n";
      std::abort();
    }
    if (addr2line_pid > 0) {  // parent is done.
      close(stdin_pipe[PIPE_READ]);
      close(stdout_pipe[PIPE_WRITE]);
      addr2line_stdin = stdin_pipe[PIPE_WRITE];
      addr2line_stdout = stdout_pipe[PIPE_READ];

      int flags = fcntl(addr2line_stdout, F_GETFL, 0);
      if (flags < 0) {
        std::cerr << "Failed to set addr2line stdout non-blocking."
                  << "fcntl(addr2line_stdout, F_GETFL, 0): errno=" << errno
                  << " (" << strerror(errno) << ")\n";
        std::abort();
      }
      flags |= O_NONBLOCK;
      int status = fcntl(addr2line_stdout, F_SETFL, flags);
      if (status < 0) {
        std::cerr << "Failed to set addr2line stdout non-blocking."
                  << "fcntl(addr2line_stdout, F_SETFL, flags): errno=" << errno
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

    const char* args[] = {"/usr/bin/addr2line",     "-C",   "-f", "-e",
                          binary_file_path.c_str(), nullptr};
    if (execv("/usr/bin/addr2line", (char* const*)args) != 0) {
      std::cerr << "Failed to execute stacktrace symbolizer process"
                << " errno=" << errno << " (" << strerror(errno) << ")\n";
      std::abort();
    }
  }

  ~StacktraceSymbolizerImpl() final {
    kill(addr2line_pid, SIGKILL);
    close(addr2line_stdin);
    close(addr2line_stdout);
  }

  void symbolize_stacktrace(RawBacktrace stack_trace, std::ostream& out) final {
    std::stringstream addr2line_command_builder;
    for (const auto& pc : stack_trace) {
      if (pc != 0) {
        addr2line_command_builder
            << "0x" << std::hex << std::setfill('0') << std::setw(16)
            << pc - dump_file_header.program_load_addr << "\n";
      }
    }
    std::string addr2line_command = addr2line_command_builder.str();
    size_t total_written = 0;
    while (total_written < addr2line_command.size()) {
      ssize_t written =
          write(addr2line_stdin, addr2line_command.data() + total_written,
                addr2line_command.size() - total_written);
      if (written < 0) {
        std::cerr << "Failed to write input to stacktrace symbolizer"
                  << " errno=" << errno << " (" << strerror(errno) << ")\n";
        std::abort();
      }
      total_written += written;
    }

    int num_lines_expected =
        2 * std::count_if(std::begin(stack_trace), std::end(stack_trace),
                          [](auto pc) {
                            return pc != 0;
                          });

    int num_lines = 0;
    char ch;
    int num_consecutive_read_failures = 0;
    std::stringstream formatter;
    formatter << "\n\t\t";
    while (num_consecutive_read_failures < 20000) {
      errno = 0;
      auto read_status = read(addr2line_stdout, &ch, 1);
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
      if (ch == '\n' && num_lines % 2 == 0) {
        formatter << ' ';
      } else {
        formatter << ch;
      }
      if (ch == '\n') {
        num_lines += 1;
        if (num_lines == num_lines_expected) {
          break;
        }
        if (num_lines % 2 == 0) {
          formatter << "\t\t";
        }
      }
    }
    if (num_consecutive_read_failures == 20000) {
      out << "\n\t\tFailed to symbolize stacktrace.";
    } else {
      out << formatter.str();
    }
  }

private:
  int addr2line_pid;
  int addr2line_stdin;
  int addr2line_stdout;
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
