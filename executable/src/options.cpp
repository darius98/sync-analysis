#include "options.hpp"

#include <algorithm>
#include <iostream>

#include <mcga/cli.hpp>

#include "debug.hpp"

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

constexpr auto analyzer_search_paths_arg_description =
    "\n"
    "\t\tExtra paths to search for analyzers. The current working\n"
    "\t\tdirectory, the sub-directory 'syan-analyzers' in the current "
    "directory\n"
    "\t\t(if it exists) and '/usr/local/lib/syan-analyzers' (if it exists) "
    "will\n"
    "\t\talways be searched.";

constexpr auto analyzer_rules_arg_description =
    "\n"
    "\t\tA list of patterns to filter analyzers based on their names.\n"
    "\t\tPatterns can contain '*' characters to denote a group of \n"
    "\t\tarbitrary characters. Patterns starting with '-' are negative.\n"
    "\t\tFor example, './sync_analysis -a -* -a mutex-*' will enable all\n"
    "\t\tanalyzers starting with 'mutex-', while disabling all other\n"
    "\t\tanalyzers. The last rule to match an analyzer determines\n"
    "\t\twhether the analyzer is enabled or disabled. If no rule matches\n"
    "\t\tan analyzer name, that analyzer is not enabled.";

constexpr auto debug_flag_description = "\n"
                                        "\t\tEnable debug logging.";

constexpr auto output_path_arg_description =
    "\n"
    "\t\tPath to a file where to report outputs. By default, reports are\n"
    "\t\twritten to stdout.";

constexpr auto print_header_flag_description =
    "\n"
    "\t\tPrint some execution metadata to STDOUT before running analysis.";

namespace syan {

Options::Options(int argc, char** argv) {
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
  auto analyzer_search_paths_arg = parser.add_list_argument(
      mcga::cli::ListArgumentSpec("analyzer-search-path")
          .set_short_name("A")
          .set_description(analyzer_search_paths_arg_description)
          .set_default_value({})
          .set_implicit_value({}));
  auto analyzer_name_rules_arg = parser.add_list_argument(
      mcga::cli::ListArgumentSpec("analyzer")
          .set_short_name("a")
          .set_description(analyzer_rules_arg_description)
          .set_default_value({"*"})
          .set_implicit_value({"*"}));
  auto debug_flag = parser.add_flag(
      mcga::cli::FlagSpec("debug").set_short_name("d").set_description(
          debug_flag_description));
  auto report_file_path_arg =
      parser.add_argument(mcga::cli::ArgumentSpec("report-file")
                              .set_short_name("r")
                              .set_description(output_path_arg_description)
                              .set_default_value("STDOUT")
                              .set_implicit_value("STDOUT"));
  auto print_header_flag =
      parser.add_flag(mcga::cli::FlagSpec("print-header")
                          .set_description(print_header_flag_description));

  auto positional_args = parser.parse(argc, argv);

  binary_file_path = std::nullopt;
  if (binary_arg->appeared()) {
    binary_file_path = binary_arg->get_value();
  }

  auto analyzer_search_path_strings = analyzer_search_paths_arg->get_value();
  analyzer_search_paths = {
      ".",
      "./syan-analyzers",
      "/usr/local/lib/syan-analyzers",
  };
  for (const auto& path_str : analyzer_search_path_strings) {
    analyzer_search_paths.emplace_back(path_str);
  }

  analyzer_name_rules = analyzer_name_rules_arg->get_value();
  std::reverse(analyzer_name_rules.begin(), analyzer_name_rules.end());

  debug_mode = debug_flag->get_value();

  auto report_file_path_value = report_file_path_arg->get_value();
  if (report_file_path_value == "STDOUT") {
    report_file_path = std::nullopt;
  } else {
    report_file_path = report_file_path_value;
  }

  print_header = print_header_flag->get_value();

  if (positional_args.size() != 2) {
    FATAL_OUT << "Invalid number of arguments.\n\n" << parser.render_help();
  }
  dump_file_path = positional_args[1];
}

}  // namespace syan
