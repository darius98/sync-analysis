#ifndef SYNC_ANALYSIS_EXECUTABLE_SRC_ANALYZER_H_
#define SYNC_ANALYSIS_EXECUTABLE_SRC_ANALYZER_H_

#include <memory>
#include <string>

namespace syan {

class Analyzer {
  using func = void (*)();

public:
  Analyzer(std::string name, void* dso_handle);

  Analyzer(const Analyzer&) = delete;
  Analyzer& operator=(const Analyzer&) = delete;

  Analyzer(Analyzer&&) noexcept = default;
  Analyzer& operator=(Analyzer&&) noexcept = default;

  ~Analyzer() = default;

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
