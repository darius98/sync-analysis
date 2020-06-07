#include "analyzer.hpp"

#include <dlfcn.h>

namespace syan {

Analyzer::Analyzer(std::string name, void* dso_handle)
    : name{std::move(name)},
      dso_handle{dso_handle},
      start_up_impl{find_func_in_dso("syan_analyzer_start_up")},
      on_event_impl{find_func_in_dso("syan_analyzer_on_event")},
      shut_down_impl{find_func_in_dso("syan_analyzer_shut_down")} {}

std::string_view Analyzer::get_name() const noexcept {
  return name;
}

void Analyzer::start_up() const noexcept {
  if (start_up_impl != nullptr) {
    start_up_impl();
  }
}

void Analyzer::on_event() const noexcept {
  if (on_event_impl != nullptr) {
    on_event_impl();
  }
}

void Analyzer::shut_down() const noexcept {
  if (shut_down_impl != nullptr) {
    shut_down_impl();
  }
}

Analyzer::func Analyzer::find_func_in_dso(const char* symbol) const noexcept {
  return reinterpret_cast<func>(dlsym(dso_handle.get(), symbol));
}

void Analyzer::DsoClose::operator()(void* handle) const noexcept {
  if (handle != nullptr) {
    dlclose(handle);
  }
}

}  // namespace syan
