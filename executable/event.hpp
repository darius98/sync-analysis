#ifndef SYNC_ANALYSIS_EXE_EVENT_H_
#define SYNC_ANALYSIS_EXE_EVENT_H_

#include <cstdint>

#include <string_view>

#include "lib_compat.hpp"

namespace syan {

constexpr std::string_view thread_object_type = "thread";
constexpr std::string_view mutex_object_type = "mutex";
constexpr std::string_view rec_mutex_object_type = "recursive mutex";
constexpr std::string_view rwlock_object_type = "read-write lock";

enum class ObjectType {
  thread,
  mutex,
  rec_mutex,
  rwlock,
};

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

  bool is_create_event() const noexcept;

  bool is_destroy_event() const noexcept;

  ObjectType object_type() const noexcept;

  static std::string_view object_type_str(ObjectType object_type) noexcept;

  std::string_view object_type_str() const noexcept;

  std::string_view type_str() const noexcept;

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
