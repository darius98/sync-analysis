#include <mcga/cli.hpp>

#include "environment.hpp"

int main(int argc, char** argv) {
  constexpr auto cli_message_header =
      "Synchronization primitives analysis version " SYNC_ANALYSIS_VERSION "\n"
      "\n"
      "Usage: ./sync_analysis [-options] [path/to/binary] "
      "[path/to/sync_analysis.dump]\n\n"
      "Use -h option to display help.";
  mcga::cli::Parser parser{cli_message_header};

  parser.addHelpFlag();
  parser.addTerminalFlag(
      mcga::cli::FlagSpec("version").setShortName("v").setDescription(
          "Display program version and usage. Does not require positional "
          "arguments and does not perform any actions."),
      cli_message_header);

  auto positional_args = parser.parse(argc, argv);

  if (positional_args.size() != 3) {
    std::cout << "Invalid number of arguments.\n"
                 "\n"
                 "Usage: ./sync_analysis [-options] [path/to/binary] "
                 "[path/to/sync_analysis.dump]";
    for (auto& arg : positional_args) {
      std::cout << arg << "\n";
    }
    return 1;
  }

  const auto& binary_file_path = positional_args[1];
  const auto& dump_file_path = positional_args[2];

  auto syan_environment = syan::Environment{binary_file_path, dump_file_path};

  syan_environment.analyze();

  return 0;
}
