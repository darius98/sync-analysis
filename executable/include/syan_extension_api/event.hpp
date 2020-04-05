#ifndef SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_EXTENSION_API_EVENT_H_
#define SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_EXTENSION_API_EVENT_H_

#include <cstdint>

#include <string_view>

namespace syan {

enum : int {
  event_type_thread = 1 << 0,
  event_type_mutex = 1 << 1,
  event_type_rec_mutex = 1 << 2,
  event_type_rwlock = 1 << 3,

  event_type_create = 1 << 4,
  event_type_try_wr_lock = 1 << 5,
  event_type_before_wr_lock = 1 << 6,
  event_type_after_wr_lock = 1 << 7,
  event_type_wr_unlock = 1 << 8,
  event_type_try_rd_lock = 1 << 9,
  event_type_before_rd_lock = 1 << 10,
  event_type_after_rd_lock = 1 << 11,
  event_type_rd_unlock = 1 << 12,
  event_type_destroy = 1 << 13,
  event_type_join = 1 << 14,
  event_type_detach = 1 << 15,
};

enum class EventType : int {
  thread_on_create = event_type_thread | event_type_create,
  thread_on_join = event_type_thread | event_type_join,
  thread_on_detach = event_type_thread | event_type_detach,

  mutex_on_create = event_type_mutex | event_type_create,
  mutex_on_try_lock = event_type_mutex | event_type_try_wr_lock,
  mutex_before_lock = event_type_mutex | event_type_before_wr_lock,
  mutex_after_lock = event_type_mutex | event_type_after_wr_lock,
  mutex_on_unlock = event_type_mutex | event_type_wr_unlock,
  mutex_on_destroy = event_type_mutex | event_type_destroy,

  rec_mutex_on_create = event_type_rec_mutex | event_type_create,
  rec_mutex_on_try_lock = event_type_rec_mutex | event_type_try_wr_lock,
  rec_mutex_before_lock = event_type_rec_mutex | event_type_before_wr_lock,
  rec_mutex_after_lock = event_type_rec_mutex | event_type_after_wr_lock,
  rec_mutex_on_unlock = event_type_rec_mutex | event_type_wr_unlock,
  rec_mutex_on_destroy = event_type_rec_mutex | event_type_destroy,

  rwlock_on_create = event_type_rwlock | event_type_create,
  rwlock_on_try_rd_lock = event_type_rwlock | event_type_try_rd_lock,
  rwlock_before_rd_lock = event_type_rwlock | event_type_before_rd_lock,
  rwlock_after_rd_lock = event_type_rwlock | event_type_after_rd_lock,
  rwlock_on_rd_unlock = event_type_rwlock | event_type_rd_unlock,
  rwlock_on_try_wr_lock = event_type_rwlock | event_type_try_wr_lock,
  rwlock_before_wr_lock = event_type_rwlock | event_type_before_wr_lock,
  rwlock_after_wr_lock = event_type_rwlock | event_type_after_wr_lock,
  rwlock_on_wr_unlock = event_type_rwlock | event_type_wr_unlock,
  rwlock_on_destroy = event_type_rwlock | event_type_destroy,
};

enum class ObjectType {
  thread,
  mutex,
  rec_mutex,
  rwlock,
  unknown,
};

constexpr std::string_view thread_object_type_str = "thread";
constexpr std::string_view mutex_object_type_str = "mutex";
constexpr std::string_view rec_mutex_object_type_str = "recursive mutex";
constexpr std::string_view rwlock_object_type_str = "read-write lock";
constexpr std::string_view unknown_str = "unknown";

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

  EventType type() const noexcept;

  int raw_type() const noexcept {
    return static_cast<int>(type());
  }

  ObjectId object() const noexcept;

  ObjectId thread() const noexcept;

  std::int64_t time_rel_to_program_start() const noexcept;

  RawBacktrace raw_backtrace() const noexcept;

  bool is_create_event() const noexcept {
    return (raw_type() & event_type_create) != 0;
  }

  bool is_destroy_event() const noexcept {
    return (raw_type() & event_type_destroy) != 0;
  }

  ObjectType object_type() const noexcept {
    if (raw_type() & event_type_thread) {
      return ObjectType::thread;
    }
    if (raw_type() & event_type_mutex) {
      return ObjectType::mutex;
    }
    if (raw_type() & event_type_rec_mutex) {
      return ObjectType::rec_mutex;
    }
    if (raw_type() & event_type_rwlock) {
      return ObjectType::rwlock;
    }
    return ObjectType::unknown;
  }

  static std::string_view object_type_str(ObjectType object_type) noexcept {
    switch (object_type) {
    case ObjectType::thread: return thread_object_type_str;
    case ObjectType::mutex: return mutex_object_type_str;
    case ObjectType::rec_mutex: return rec_mutex_object_type_str;
    case ObjectType::rwlock: return rwlock_object_type_str;
    case ObjectType::unknown: return unknown_str;
    }
  }

  std::string_view object_type_str() const noexcept {
    return object_type_str(object_type());
  }

  std::string_view type_str() const noexcept {
#define EVENT_TYPE_STR_CASE(E)                                                 \
  case EventType::E: return #E;
    switch (type()) {
      EVENT_TYPE_STR_CASE(thread_on_create)
      EVENT_TYPE_STR_CASE(thread_on_join)
      EVENT_TYPE_STR_CASE(thread_on_detach)
      EVENT_TYPE_STR_CASE(mutex_on_create)
      EVENT_TYPE_STR_CASE(mutex_before_lock)
      EVENT_TYPE_STR_CASE(mutex_after_lock)
      EVENT_TYPE_STR_CASE(mutex_on_try_lock)
      EVENT_TYPE_STR_CASE(mutex_on_unlock)
      EVENT_TYPE_STR_CASE(mutex_on_destroy)
      EVENT_TYPE_STR_CASE(rec_mutex_on_create)
      EVENT_TYPE_STR_CASE(rec_mutex_before_lock)
      EVENT_TYPE_STR_CASE(rec_mutex_after_lock)
      EVENT_TYPE_STR_CASE(rec_mutex_on_try_lock)
      EVENT_TYPE_STR_CASE(rec_mutex_on_unlock)
      EVENT_TYPE_STR_CASE(rec_mutex_on_destroy)
      EVENT_TYPE_STR_CASE(rwlock_on_create)
      EVENT_TYPE_STR_CASE(rwlock_before_rd_lock)
      EVENT_TYPE_STR_CASE(rwlock_after_rd_lock)
      EVENT_TYPE_STR_CASE(rwlock_on_rd_unlock)
      EVENT_TYPE_STR_CASE(rwlock_on_try_rd_lock)
      EVENT_TYPE_STR_CASE(rwlock_before_wr_lock)
      EVENT_TYPE_STR_CASE(rwlock_after_wr_lock)
      EVENT_TYPE_STR_CASE(rwlock_on_wr_unlock)
      EVENT_TYPE_STR_CASE(rwlock_on_try_wr_lock)
      EVENT_TYPE_STR_CASE(rwlock_on_destroy)
    default: return unknown_str;
    }
#undef EVENT_TYPE_STR_CASE
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
