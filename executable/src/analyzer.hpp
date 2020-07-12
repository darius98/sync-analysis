#ifndef SYNC_ANALYSIS_EXECUTABLE_SRC_ANALYZER_H_
#define SYNC_ANALYSIS_EXECUTABLE_SRC_ANALYZER_H_

#include <memory>
#include <string>

namespace syan {

struct DsoClose {
  void operator()(void* dso_handle) const noexcept;
};
using DsoHandle = std::unique_ptr<void, DsoClose>;

class Analyzer {
public:
  Analyzer(std::string name, std::string dso_file_path, DsoHandle dso_handle);

  Analyzer(const Analyzer&) = delete;
  Analyzer& operator=(const Analyzer&) = delete;

  Analyzer(Analyzer&&) noexcept = default;
  Analyzer& operator=(Analyzer&&) noexcept = default;

  ~Analyzer() = default;

  std::string_view get_name() const noexcept;

  std::string_view get_dso_file_path() const noexcept;

  void start_up() const noexcept;

  void on_event() const noexcept;

  void shut_down() const noexcept;

private:
  using func = void (*)();

  func find_func_in_dso(const char* symbol) const noexcept;

  std::string name;
  std::string dso_file_path;
  DsoHandle dso_handle;
  func start_up_impl;
  func on_event_impl;
  func shut_down_impl;
};

}  // namespace syan

#endif
