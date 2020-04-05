#ifndef SYNC_ANALYSIS_EXECUTABLE_SRC_OPTIONS_H_
#define SYNC_ANALYSIS_EXECUTABLE_SRC_OPTIONS_H_

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace syan {

struct Options {
  std::optional<std::string> binary_file_path;
  std::vector<std::filesystem::path> extension_search_paths;
  std::vector<std::string> extension_name_rules;
  bool debug_mode;
  std::optional<std::filesystem::path> report_file_path;
  bool print_header;
  std::filesystem::path dump_file_path;

  Options(int argc, char** argv);
};

}  // namespace syan

#endif
