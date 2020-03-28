#include <iostream>

#include <mcga/cli.hpp>

#include "find_extensions.hpp"
#include "run_analysis.hpp"

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

  parser.add_terminal_flag(
      mcga::cli::FlagSpec("help").set_short_name("h").set_description(
          help_description),
      [&parser]() {
        std::cout << help_menu_header << parser.render_help();
      });
  parser.add_terminal_flag(
      mcga::cli::FlagSpec("version").set_short_name("v").set_description(
          version_description),
      cli_message_header);
  auto binary_arg =
      parser.add_argument(mcga::cli::ArgumentSpec("binary")
                              .set_short_name("b")
                              .set_description(binary_arg_description)
                              .set_default_value("UNKNOWN"));

  auto positional_args = parser.parse(argc, argv);

  if (positional_args.size() != 2) {
    std::cout << "Invalid number of arguments.\n\n" << parser.render_help();
    return 1;
  }

  std::optional<std::string> binary_file_path = std::nullopt;
  if (binary_arg->appeared()) {
    binary_file_path = binary_arg->get_value();
  }

  auto extensions = syan::find_extensions({
      ".",
      "./syan-ext",
      "/usr/local/lib/syan-ext",
  });
  syan::run_analysis(std::move(binary_file_path), positional_args[1],
                     extensions);

  return 0;
}
