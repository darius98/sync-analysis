#include <filesystem>
#include <iostream>

#include <errno.h>
#include <signal.h>
#include <unistd.h>

#include <mcga/cli.hpp>

namespace cli = mcga::cli;

int main(int argc, char** argv) {
  cli::Parser parser("Sync Analysis extension test runner.");

  auto test_binary_arg = parser.add_argument(cli::ArgumentSpec("test_binary"));
  auto sync_analysis_binary_arg =
      parser.add_argument(cli::ArgumentSpec("sync_analysis_binary"));
  auto sync_analysis_extension_dir_arg =
      parser.add_argument(cli::ArgumentSpec("sync_analysis_extension_dir"));
  auto sync_analysis_extension_name_arg =
      parser.add_argument(cli::ArgumentSpec("sync_analysis_extension_name"));
  auto sync_analysis_output_file_arg =
      parser.add_argument(cli::ArgumentSpec("sync_analysis_output_file"));

  parser.parse(argc, argv);

  auto test_binary = test_binary_arg->get_value();
  auto sync_analysis_binary = sync_analysis_binary_arg->get_value();
  auto sync_analysis_extension_dir =
      sync_analysis_extension_dir_arg->get_value();
  auto sync_analysis_extension_name =
      sync_analysis_extension_name_arg->get_value();
  auto sync_analysis_output_file = sync_analysis_output_file_arg->get_value();

  int child = fork();
  if (child < 0) {
    std::cerr << "Fork failed for test binary process. return=" << child
              << ", errno=" << errno << "\n";

    kill(getpid(), SIGUSR1);
  }
  if (child == 0) {
    char* const args[]{test_binary.data(), nullptr};
    std::cout << "Executing test binary: " << test_binary.c_str() << "\n";
    int status = execv(test_binary.c_str(), args);
    std::cerr << "execv failed for test binary process.  return=" << status
              << ", errno=" << errno << "\n";
    kill(getpid(), SIGUSR1);
  }

  int exit_status = -22;
  for (int num_waits = 0; num_waits < 10000; num_waits++) {
    int wait_status;
    int ret = waitpid(child, &wait_status, WNOHANG);
    if (ret < 0) {
      std::cerr << "waitpid failed for test binary process. return=" << ret
                << ", errno=" << errno << ", wait_status=" << wait_status
                << "\n";

      kill(getpid(), SIGUSR1);
    }
    if (ret > 0) {
      if (WIFSIGNALED(wait_status)) {
        std::cerr << "test binary exited with signal. signal="
                  << WTERMSIG(wait_status) << "\n";

        kill(getpid(), SIGUSR1);
      }
      if (WIFSTOPPED(wait_status)) {
        std::cerr << "test binary stopped with signal. signal="
                  << WSTOPSIG(wait_status) << "\n";

        kill(getpid(), SIGUSR1);
      }
      exit_status = WEXITSTATUS(wait_status);
      break;
    }
    usleep(100);
  }
  if (exit_status == -22) {
    int kill_status = ::kill(child, SIGKILL);
    if (kill_status < 0 && errno != ESRCH) {
      std::cerr << "could not kill test binary after one second. return="
                << kill_status << ", errno=" << errno << "\n";

      kill(getpid(), SIGUSR1);
    }
    if (kill_status < 0 && errno == ESRCH) {
      int wait_status;
      int ret = waitpid(child, &wait_status, WNOHANG);
      if (ret < 0) {
        std::cerr << "waitpid failed for test binary process. return=" << ret
                  << ", errno=" << errno << ", wait_status=" << wait_status
                  << "\n";

        kill(getpid(), SIGUSR1);
      }
      if (ret > 0) {
        if (WIFSIGNALED(wait_status)) {
          std::cerr << "test binary exited with signal. signal="
                    << WTERMSIG(wait_status) << "\n";

          kill(getpid(), SIGUSR1);
        }
        if (WIFSTOPPED(wait_status)) {
          std::cerr << "test binary stopped with signal. signal="
                    << WSTOPSIG(wait_status) << "\n";

          kill(getpid(), SIGUSR1);
        }
        exit_status = WEXITSTATUS(wait_status);
      }
    }
    if (exit_status != 0) {
      std::cerr << "test binary exited with non-zero status. exit_status="
                << exit_status << "\n";

      kill(getpid(), SIGUSR1);
    }
  }

  auto sync_analysis_dump_path =
      std::filesystem::absolute("sync_analysis.dump").string();

  char* const args[]{sync_analysis_binary.data(),
                     (char*)"-b",
                     test_binary.data(),
                     (char*)"-E",
                     sync_analysis_extension_dir.data(),
                     (char*)"--print-header",
                     (char*)"-e",
                     sync_analysis_extension_name.data(),
                     (char*)"-o",
                     sync_analysis_output_file.data(),
                     sync_analysis_dump_path.data(),
                     nullptr};
  std::cout << "Executing sync analysis binary: " << sync_analysis_binary
            << " -b " << test_binary << " -E " << sync_analysis_extension_dir
            << " --print-header -e " << sync_analysis_extension_name << " -o "
            << sync_analysis_output_file << " " << sync_analysis_dump_path
            << std::endl;
  int status = execv(sync_analysis_binary.c_str(), args);
  std::cerr << "execv failed for sync analysis process. return=" << status
            << ", errno=" << errno << "\n";

  kill(getpid(), SIGUSR1);
}
