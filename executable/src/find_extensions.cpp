#include "find_extensions.hpp"

#include <dlfcn.h>

namespace {

bool is_extension_filename(const std::filesystem::path& path) {
#ifdef SYNC_ANALYSIS_IS_MAC_OS_X
  return path.extension() == ".dylib" &&
         std::string_view{path.filename().c_str()}.substr(0, 11) ==
             "libsyanext-";
#elif SYNC_ANALYSIS_IS_LINUX
  return path.extension() == ".so" &&
         std::string_view{path.filename().c_str()}.substr(0, 11) ==
             "libsyanext-";
#endif
}

bool respects_extension_name_rule(std::string_view name,
                                  std::string_view rule) {
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

bool respects_extension_name_rules(std::string_view name,
                                   const std::vector<std::string>& rules) {
  for (const auto& rule : rules) {
    if (!rule.empty() && respects_extension_name_rule(name, rule)) {
      return rule[0] != '-';
    }
  }
  return false;
}

}  // namespace

namespace syan {

std::vector<Extension>
find_extensions(const std::vector<std::filesystem::path>& directories,
                const std::vector<std::string>& rules) {
  std::vector<Extension> extensions;
  for (const auto& directory : directories) {
    if (!std::filesystem::is_directory(directory)) {
      continue;
    }
    for (const auto& relative_path :
         std::filesystem::directory_iterator(directory)) {
      auto path = std::filesystem::absolute(relative_path);
      if (!std::filesystem::is_regular_file(path) ||
          !is_extension_filename(path)) {
        continue;
      }
      void* handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
      if (handle == nullptr) {
        continue;
      }
      void* syan_extension_name_symbol = dlsym(handle, "syan_extension");
      if (syan_extension_name_symbol == nullptr) {
        dlclose(handle);
        continue;
      }
      std::string_view syan_extension_name =
          *static_cast<const char**>(syan_extension_name_symbol);
      if (respects_extension_name_rules(syan_extension_name, rules)) {
        extensions.emplace_back(std::string{syan_extension_name}, handle);
      }
    }
  }
  return extensions;
}

}  // namespace syan
