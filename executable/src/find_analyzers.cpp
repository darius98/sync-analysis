#include "find_analyzers.hpp"

#include <filesystem>
#include <string>

#include <dlfcn.h>

#include "debug.hpp"

namespace {

bool is_analyzer_filename(const std::filesystem::path& path) {
  return std::string_view{path.filename().c_str()}.substr(0, 17) ==
         "libsyan-analyzer-";
}

bool respects_analyzer_name_rule(std::string_view name, std::string_view rule) {
  if (rule[0] == '-') {
    rule.remove_prefix(1);
  }
  if (rule.empty()) {
    return false;
  }
  std::vector<std::vector<bool>> dp(rule.size() + 1,
                                    std::vector<bool>(name.size() + 1, false));
  dp[0][0] = true;
  if (rule[0] == '*') {
    dp[1][0] = true;
  }
  for (std::size_t i = 0; i < rule.size(); i++) {
    for (std::size_t j = 0; j < name.size(); j++) {
      dp[i + 1][j + 1] =
          (rule[i] == '*' && (dp[i][j] || dp[i + 1][j] || dp[i][j + 1])) ||
          (rule[i] == name[j] && dp[i][j]);
    }
  }

  return dp[rule.size()][name.size()];
}

bool respects_analyzer_name_rules(std::string_view name,
                                  const std::vector<std::string>& rules) {
  for (const auto& rule : rules) {
    if (!rule.empty() && respects_analyzer_name_rule(name, rule)) {
      return rule[0] != '-';
    }
  }
  return true;
}

}  // namespace

namespace syan {

std::vector<Analyzer> find_analyzers(const Options& options) {
  std::vector<Analyzer> analyzers;
  for (const auto& directory : options.analyzer_search_paths) {
    if (!std::filesystem::is_directory(directory)) {
      DOUT << "Path " << directory.string() << " is not a directory, skipping.";
      continue;
    }
    for (const auto& relative_path :
         std::filesystem::directory_iterator(directory)) {
      auto path = std::filesystem::absolute(relative_path);
      if (!std::filesystem::is_regular_file(path) ||
          !is_analyzer_filename(path)) {
        continue;
      }
      DsoHandle handle(dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL));
      if (handle == nullptr) {
        continue;
      }
      void* syan_analyzer_name_symbol = dlsym(handle.get(), "syan_analyzer");
      if (syan_analyzer_name_symbol == nullptr) {
        continue;
      }
      std::string_view syan_analyzer_name =
          *static_cast<const char**>(syan_analyzer_name_symbol);
      if (respects_analyzer_name_rules(syan_analyzer_name,
                                       options.analyzer_name_rules)) {
        analyzers.emplace_back(std::string{syan_analyzer_name}, path.string(),
                               std::move(handle));
      }
    }
  }
  return analyzers;
}

}  // namespace syan
