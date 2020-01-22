#ifndef SYNC_ANALYSIS_EXE_EVENT_H_
#define SYNC_ANALYSIS_EXE_EVENT_H_

#include <cstdint>

#include "lib_compat.hpp"

namespace syan {

// A shared pointer-style implementation that is:
// - without thread-safety guarantees
// - without support for weak_ptr
// - without support for taking ownership of an existing pointer
class Event {
public:
  static Event make(const ::SyanEvent& event);

  explicit Event() noexcept;
  Event(decltype(nullptr)) noexcept;

  Event(const Event& other) noexcept;
  Event(Event&& other) noexcept;

  Event& operator=(const Event& other) noexcept;
  Event& operator=(Event&& other) noexcept;

  ~Event() noexcept;

  EventType type() const noexcept;

  ObjectId object() const noexcept;

  ObjectId thread() const noexcept;

  std::int64_t time_rel_to_program_start() const noexcept;

  RawBacktrace raw_backtrace() const noexcept;

  explicit operator bool() const noexcept;

  bool operator==(decltype(nullptr)) const noexcept;
  bool operator!=(decltype(nullptr)) const noexcept;

  bool operator<(const Event& other) const noexcept;
  bool operator>(const Event& other) const noexcept;
  bool operator<=(const Event& other) const noexcept;
  bool operator>=(const Event& other) const noexcept;
  bool operator==(const Event& other) const noexcept;
  bool operator!=(const Event& other) const noexcept;

private:
  struct EventPtrInternal {
    int ref_count = 1;
    ::SyanEvent event;
  };

  EventPtrInternal* ptr;
};

} // namespace syan

#endif
