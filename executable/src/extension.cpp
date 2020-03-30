#include "extension.hpp"

#include <dlfcn.h>

namespace {}

namespace syan {

Extension::Extension(std::string name, void* dso_handle)
    : name{std::move(name)},
      dso_handle{dso_handle},
      context{nullptr},
      start_up_impl{find_func_in_dso("syan_extension_start_up")},
      on_event_impl{find_func_in_dso("syan_extension_on_event")},
      shut_down_impl{find_func_in_dso("syan_extension_shut_down")} {}

Extension::Extension(std::string name, void* context, func start_up_impl,
                     func on_event_impl, func shut_down_impl)
    : name{std::move(name)},
      dso_handle{nullptr},
      context{context},
      start_up_impl{start_up_impl},
      on_event_impl{on_event_impl},
      shut_down_impl{shut_down_impl} {}

std::string_view Extension::get_name() const noexcept {
  return name;
}

void Extension::start_up() const noexcept {
  if (start_up_impl != nullptr) {
    start_up_impl(context);
  }
}

void Extension::on_event() const noexcept {
  if (on_event_impl != nullptr) {
    on_event_impl(context);
  }
}

void Extension::shut_down() const noexcept {
  if (shut_down_impl != nullptr) {
    shut_down_impl(context);
  }
}

Extension::func Extension::find_func_in_dso(const char* symbol) const noexcept {
  return reinterpret_cast<func>(dlsym(dso_handle.get(), symbol));
}

void Extension::DsoClose::operator()(void* handle) const noexcept {
  if (handle != nullptr) {
    dlclose(handle);
  }
}

}  // namespace syan
