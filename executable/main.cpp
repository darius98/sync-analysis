#include <iostream>

#include <mcga/cli.hpp>

#include "environment.hpp"

constexpr auto help_menu_header =
    "Synchronization primitives analysis version " SYNC_ANALYSIS_VERSION "\n\n";

constexpr auto cli_message_header =
    "Usage: ./sync_analysis [-options] [path/to/sync_analysis.dump]\n"
    "Options:";

constexpr auto help_description = "\n\t\tDisplay this help menu.";

constexpr auto version_description =
    "\n"
    "\t\tDisplay program version and usage. Does not require positional\n"
    "\t\targuments and does not perform any actions.";

constexpr auto binary_arg_description =
    "\n"
    "\t\tPath to original binary file. Providing this will include symbol\n"
    "\t\tnames in stack traces of reports. If the binary contains debug\n"
    "\t\tsymbols (or, on Mac OS, a .dSYM file is available next to the\n"
    "\t\tbinary), file and line information will also be printed where\n"
    "\t\tavailable.";

int main(int argc, char** argv) {
  mcga::cli::Parser parser{cli_message_header};

  parser.addTerminalFlag(
      mcga::cli::FlagSpec("help").setShortName("h").setDescription(
          help_description),
      [&parser]() {
        std::cout << help_menu_header << parser.renderHelp();
      });
  parser.addTerminalFlag(
      mcga::cli::FlagSpec("version").setShortName("v").setDescription(
          version_description),
      cli_message_header);
  auto binary_arg =
      parser.addArgument(mcga::cli::ArgumentSpec("binary")
                             .setShortName("b")
                             .setDescription(binary_arg_description)
                             .setDefaultValue("UNKNOWN"));

  auto positional_args = parser.parse(argc, argv);

  if (positional_args.size() != 2) {
    std::cout << "Invalid number of arguments.\n\n" << parser.renderHelp();
    return 1;
  }

  std::optional<std::string> binary_file_path = std::nullopt;
  if (binary_arg->appeared()) {
    binary_file_path = binary_arg->getValue();
  }

  auto env = syan::Environment{binary_file_path, positional_args[1]};

  env.analyze();

  return 0;
}
