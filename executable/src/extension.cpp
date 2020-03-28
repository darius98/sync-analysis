#include "extension.hpp"

#include <iostream>

#include <dlfcn.h>

namespace syan {

Extension::Extension(const std::string& extension_name,
                     const std::vector<std::filesystem::path>& dso_search_paths)
    : name(extension_name) {
  for (const auto& directory : dso_search_paths) {
#ifdef SYNC_ANALYSIS_IS_MAC_OS_X
#define SYNC_ANALYSIS_DSO_EXTENSION ".dylib"
#else
#define SYNC_ANALYSIS_DSO_EXTENSION ".so"
#endif
    std::filesystem::path dso_path =
        directory /
        ("libsyan_ext_" + extension_name + SYNC_ANALYSIS_DSO_EXTENSION);
#undef SYNC_ANALYSIS_DSO_EXTENSION
    std::string dso_path_str = dso_path.string();
    void* handle = dlopen(dso_path_str.c_str(), RTLD_NOW | RTLD_LOCAL);
    if (handle == nullptr) {
      continue;
    }
    void* syan_extension_name = dlsym(handle, "syan_extension");
    if (syan_extension_name == nullptr ||
        *static_cast<char**>(syan_extension_name) != name) {
      dlclose(handle);
      continue;
    }
    dso_handle.reset(handle);
    break;
  }
  if (dso_handle == nullptr) {
    std::cerr << "Error loading extension '" << extension_name << "'.\n";
    return;
  }
  start_up_impl = reinterpret_cast<func>(
      dlsym(dso_handle.get(), "syan_extension_start_up"));
  on_event_impl = reinterpret_cast<func>(
      dlsym(dso_handle.get(), "syan_extension_on_event"));
  shut_down_impl = reinterpret_cast<func>(
      dlsym(dso_handle.get(), "syan_extension_shut_down"));
  if (start_up_impl == nullptr && on_event_impl == nullptr &&
      shut_down_impl == nullptr) {
    std::cerr << "Warning: extension '" << name
              << "' implements nothing of the SYAN extension interface, and so "
                 "it does nothing.\n";
  }
}

std::string_view Extension::get_name() const noexcept {
  return name;
}

void Extension::start_up() const noexcept {
  if (start_up_impl != nullptr) {
    start_up_impl();
  }
}

void Extension::on_event() const noexcept {
  if (on_event_impl != nullptr) {
    on_event_impl();
  }
}

void Extension::shut_down() const noexcept {
  if (shut_down_impl != nullptr) {
    shut_down_impl();
  }
}

void Extension::DsoClose::operator()(void* handle) const noexcept {
  if (handle != nullptr) {
    dlclose(handle);
  }
}

}  // namespace syan
