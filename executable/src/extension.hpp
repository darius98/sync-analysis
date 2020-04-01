#ifndef SYNC_ANALYSIS_EXECUTABLE_SRC_EXTENSION_H_
#define SYNC_ANALYSIS_EXECUTABLE_SRC_EXTENSION_H_

#include <memory>
#include <string>

namespace syan {

class Extension {
  using func = void (*)();

public:
  Extension(std::string name, void* dso_handle);

  Extension(const Extension&) = delete;
  Extension& operator=(const Extension&) = delete;

  Extension(Extension&&) noexcept = default;
  Extension& operator=(Extension&&) noexcept = default;

  ~Extension() = default;

  std::string_view get_name() const noexcept;

  void start_up() const noexcept;

  void on_event() const noexcept;

  void shut_down() const noexcept;

private:
  func find_func_in_dso(const char* symbol) const noexcept;

  struct DsoClose {
    void operator()(void* dso_handle) const noexcept;
  };

  std::string name;
  std::unique_ptr<void, DsoClose> dso_handle;
  func start_up_impl;
  func on_event_impl;
  func shut_down_impl;
};

}  // namespace syan

#endif
