#ifndef SYNC_ANALYSIS_EXECUTABLE_SRC_EXTENSION_H_
#define SYNC_ANALYSIS_EXECUTABLE_SRC_EXTENSION_H_

#include <filesystem>
#include <string>
#include <vector>

namespace syan {

class Extension {
  using func = void (*)();

public:
  Extension(const std::string& extension_name,
            const std::vector<std::filesystem::path>& dso_search_paths);

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
  struct DsoClose {
    void operator()(void* dso_handle) const noexcept;
  };

  std::unique_ptr<void, DsoClose> dso_handle;
  std::string name;
  func start_up_impl{nullptr};
  func on_event_impl{nullptr};
  func shut_down_impl{nullptr};
};

}  // namespace syan

#endif
