#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>

#include <mcga/cli.hpp>

#include "debug.hpp"
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

constexpr auto extension_search_paths_arg_description =
    "\n"
    "\t\tExtra paths to search for extensions. The current working\n"
    "\t\tdirectory, the sub-directory 'syan-ext' in the current directory\n"
    "\t\t(if it exists) and '/usr/local/lib/syan-ext' (if it exists) will\n"
    "\t\talways be searched.";

constexpr auto extension_rules_arg_description =
    "\n"
    "\t\tA list of patterns to filter extensions based on their names.\n"
    "\t\tPatterns can contain '*' characters to denote a group of \n"
    "\t\tarbitrary characters. Patterns starting with '-' are negative.\n"
    "\t\tFor example, './sync_analysis -e -* -e mutex-*' will enable all\n"
    "\t\textensions starting with 'mutex-', while disabling all other\n"
    "\t\textensions. The last rule to match an extension determines\n"
    "\t\twhether the extension is enabled or disabled. If no rule matches\n"
    "\t\tan extension name, that extension is not enabled.";

constexpr auto debug_flag_description = "\n"
                                        "\t\tEnable debug logging.";

constexpr auto output_path_arg_description =
    "\n"
    "\t\tPath to a file where to report outputs. By default, reports are\n"
    "\t\twritten to stdout.";

constexpr auto print_header_flag_description =
    "\n"
    "\t\tPrint some execution metadata to STDOUT before running analysis.";

int main(int argc, char** argv) {
  syan::debugging::install_abort_handler();

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
  auto extension_search_paths_arg = parser.add_list_argument(
      mcga::cli::ListArgumentSpec("extension-search-path")
          .set_short_name("E")
          .set_description(extension_search_paths_arg_description)
          .set_default_value({})
          .set_implicit_value({}));
  auto extension_rules_arg = parser.add_list_argument(
      mcga::cli::ListArgumentSpec("extension")
          .set_short_name("e")
          .set_description(extension_rules_arg_description)
          .set_default_value({"*"})
          .set_implicit_value({"*"}));
  auto debug_flag = parser.add_flag(
      mcga::cli::FlagSpec("debug").set_short_name("d").set_description(
          debug_flag_description));
  auto output_path_arg =
      parser.add_argument(mcga::cli::ArgumentSpec("output")
                              .set_short_name("o")
                              .set_description(output_path_arg_description)
                              .set_default_value("STDOUT")
                              .set_implicit_value("STDOUT"));
  auto print_header_flag =
      parser.add_flag(mcga::cli::FlagSpec("print-header")
                          .set_description(print_header_flag_description));

  auto positional_args = parser.parse(argc, argv);

  syan::debugging::set_debug_enabled(debug_flag->get_value());

  DOUT << "Loaded command line arguments";

  if (positional_args.size() != 2) {
    std::cout << "Invalid number of arguments.\n\n" << parser.render_help();
    return 1;
  }

  std::ostream* report_stream = &std::cout;
  std::unique_ptr<std::ostream> own_report_stream;
  auto output_path = output_path_arg->get_value();
  if (output_path != "STDOUT") {
    own_report_stream.reset(new std::ofstream(output_path.c_str()));
    if (!*own_report_stream) {
      std::cout << "Cannot open path to report_file for test writing: "
                << output_path << "\n";
      return 1;
    }
    report_stream = own_report_stream.get();
  }

  std::optional<std::string> binary_file_path = std::nullopt;
  if (binary_arg->appeared()) {
    binary_file_path = binary_arg->get_value();
  }

  auto extension_search_path_strings = extension_search_paths_arg->get_value();
  std::vector<std::filesystem::path> extension_search_paths = {
      ".",
      "./syan-ext",
      "/usr/local/lib/syan-ext",
  };
  for (const auto& path_str : extension_search_path_strings) {
    extension_search_paths.emplace_back(path_str);
  }

  auto rules = extension_rules_arg->get_value();
  std::reverse(rules.begin(), rules.end());

  DOUT << "Loading extensions...";
  auto extensions = syan::find_extensions(extension_search_paths, rules);
  DOUT << "Loaded extensions:";
  for (const auto& extension : extensions) {
    DOUT << "\t* " << extension.get_name();
  }
  DOUT << "Running analysis...";
  int status = syan::run_analysis(
      std::move(binary_file_path), positional_args[1], std::move(extensions),
      report_stream, output_path, print_header_flag->get_value());
  DOUT << "Analysis done. Exit status = " << status;
  return status;
}
