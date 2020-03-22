#ifndef SYNC_ANALYSIS_EXE_EVENT_H_
#define SYNC_ANALYSIS_EXE_EVENT_H_

#include <cstdint>

#include <string_view>

namespace syan {

enum class EventType {
  thread_on_create = 0,
  thread_on_join = 1,
  thread_on_detach = 2,
  mutex_on_create = 3,
  mutex_on_try_lock = 4,
  mutex_before_lock = 5,
  mutex_after_lock = 6,
  mutex_on_unlock = 7,
  mutex_on_destroy = 8,
  rec_mutex_on_create = 9,
  rec_mutex_on_try_lock = 10,
  rec_mutex_before_lock = 11,
  rec_mutex_after_lock = 12,
  rec_mutex_on_unlock = 13,
  rec_mutex_on_destroy = 14,
  rwlock_on_create = 15,
  rwlock_on_try_rd_lock = 16,
  rwlock_before_rd_lock = 17,
  rwlock_after_rd_lock = 18,
  rwlock_on_rd_unlock = 19,
  rwlock_on_try_wr_lock = 20,
  rwlock_before_wr_lock = 21,
  rwlock_after_wr_lock = 22,
  rwlock_on_wr_unlock = 23,
  rwlock_on_destroy = 24,
};

enum class ObjectType {
  thread,
  mutex,
  rec_mutex,
  rwlock,
};

using ObjectId = std::intptr_t;

using RawBacktrace = intptr_t (&)[12];

// A shared pointer-style implementation that is:
// - without thread-safety guarantees
// - without support for weak_ptr
// - without support for taking ownership of an existing pointer
class Event {
public:
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
  static Event make(const void* syan_event);

  struct EventPtrInternal;
  friend class EventFileReader;

  EventPtrInternal* ptr;
};

Event current_event() noexcept;

}  // namespace syan

#endif
