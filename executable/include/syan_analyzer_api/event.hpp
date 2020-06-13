#ifndef SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_ANALYZER_API_EVENT_H_
#define SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_ANALYZER_API_EVENT_H_

#include <cstdint>

#include <string_view>

#include <sync_analysis.h>

namespace syan {

using ObjectId = std::intptr_t;

using RawBacktrace = intptr_t (&)[12];

// A shared pointer-style implementation that is:
// - without thread-safety guarantees
// - without support for weak_ptr
// - without support for taking ownership of an existing pointer
class Event {
public:
  Event() noexcept: ptr(nullptr) {}

  Event(decltype(nullptr)) noexcept: ptr(nullptr) {}

  Event(const Event& other) noexcept: ptr(other.ptr) {
    inc_ref_count();
  }

  Event(Event&& other) noexcept: ptr(other.ptr) {
    other.ptr = nullptr;
  }

  Event& operator=(const Event& other) noexcept {
    if (this == &other) {
      return *this;
    }

    dec_ref_count();
    ptr = other.ptr;
    inc_ref_count();
    return *this;
  }

  Event& operator=(Event&& other) noexcept {
    if (this == &other) {
      return *this;
    }

    dec_ref_count();
    ptr = other.ptr;
    other.ptr = nullptr;
    return *this;
  }

  ~Event() noexcept {
    dec_ref_count();
  }

  int type() const noexcept;

  int raw_type() const noexcept {
    return static_cast<int>(type());
  }

  ObjectId object() const noexcept;

  ObjectId thread() const noexcept;

  std::int64_t time_rel_to_program_start() const noexcept;

  RawBacktrace raw_backtrace() const noexcept;

  bool is_create_event() const noexcept {
    return (raw_type() & SA_EV_CREATE) != 0;
  }

  bool is_destroy_event() const noexcept {
    return (raw_type() & SA_EV_DESTROY) != 0;
  }

  explicit operator bool() const noexcept {
    return ptr != nullptr;
  }

  bool operator==(decltype(nullptr)) const noexcept {
    return ptr == nullptr;
  }

  bool operator!=(decltype(nullptr)) const noexcept {
    return ptr != nullptr;
  }

  bool operator<(const Event& other) const noexcept {
    return ptr < other.ptr;
  }

  bool operator>(const Event& other) const noexcept {
    return ptr > other.ptr;
  }

  bool operator<=(const Event& other) const noexcept {
    return ptr <= other.ptr;
  }

  bool operator>=(const Event& other) const noexcept {
    return ptr >= other.ptr;
  }

  bool operator==(const Event& other) const noexcept {
    return ptr == other.ptr;
  }

  bool operator!=(const Event& other) const noexcept {
    return ptr != other.ptr;
  }

private:
  static Event make(const void* syan_event);

  void inc_ref_count();
  void dec_ref_count();

  struct EventPtrInternal;
  friend class EventFileReader;

  EventPtrInternal* ptr;
};

}  // namespace syan

#endif
