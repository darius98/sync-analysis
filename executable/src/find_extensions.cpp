#include "find_extensions.hpp"

#include <dlfcn.h>

namespace {

bool is_extension_filename(const std::filesystem::path& path) {
#ifdef SYNC_ANALYSIS_IS_MAC_OS_X
  return path.extension() == ".dylib" &&
         std::string_view{path.filename().c_str()}.substr(0, 12) ==
             "libsyan_ext_";
#elif SYNC_ANALYSIS_IS_LINUX
  return path.extension() == ".so" &&
         std::string_view{path.filename().c_str()}.substr(0, 12) ==
             "libsyan_ext_";
#endif
}

}  // namespace

namespace syan {

std::vector<Extension>
find_extensions(const std::vector<std::filesystem::path>& directories) {
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
      void* syan_extension_name = dlsym(handle, "syan_extension");
      if (syan_extension_name == nullptr) {
        dlclose(handle);
        continue;
      }
      extensions.emplace_back(*static_cast<const char**>(syan_extension_name),
                              handle);
    }
  }
  return extensions;
}

}  // namespace syan
