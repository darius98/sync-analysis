#include "executable/include/check_api/event.hpp"

#include "lib_compat.hpp"

namespace {

constexpr std::string_view thread_object_type = "thread";
constexpr std::string_view mutex_object_type = "mutex";
constexpr std::string_view rec_mutex_object_type = "recursive mutex";
constexpr std::string_view rwlock_object_type = "read-write lock";

}  // namespace

namespace syan {

struct Event::EventPtrInternal {
  int ref_count = 1;
  ::SyanEvent event;
};

Event Event::make(const ::SyanEvent& event) {
  if (event.signature != SYAN_EVENT_SIGNATURE) {
    return Event{};
  }
  Event event_ptr;
  event_ptr.ptr = new EventPtrInternal{1, event};
  return event_ptr;
}

Event::Event() noexcept: ptr(nullptr) {}

Event::Event(decltype(nullptr)) noexcept: ptr(nullptr) {}

Event::Event(const Event& other) noexcept: ptr(other.ptr) {
  if (ptr != nullptr) {
    ptr->ref_count += 1;
  }
}

Event::Event(Event&& other) noexcept: ptr(other.ptr) {
  other.ptr = nullptr;
}

Event& Event::operator=(const Event& other) noexcept {
  if (this == &other) {
    return *this;
  }

  if (ptr != nullptr) {
    ptr->ref_count -= 1;
    if (ptr->ref_count == 0) {
      delete ptr;
    }
  }
  ptr = other.ptr;
  if (ptr != nullptr) {
    ptr->ref_count += 1;
  }
  return *this;
}

Event& Event::operator=(Event&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  if (ptr != nullptr) {
    ptr->ref_count -= 1;
    if (ptr->ref_count == 0) {
      delete ptr;
    }
  }
  ptr = other.ptr;
  other.ptr = nullptr;
  return *this;
}

Event::~Event() noexcept {
  if (ptr != nullptr) {
    ptr->ref_count -= 1;
    if (ptr->ref_count == 0) {
      delete ptr;
    }
  }
}

EventType Event::type() const noexcept {
  return static_cast<EventType>(ptr->event.event_type);
}

ObjectId Event::object() const noexcept {
  return ptr->event.addr;
}

ObjectId Event::thread() const noexcept {
  return ptr->event.thread_id;
}

std::int64_t Event::time_rel_to_program_start() const noexcept {
  return ptr->event.timestamp;
}

RawBacktrace Event::raw_backtrace() const noexcept {
  return ptr->event.backtrace;
}

bool Event::is_create_event() const noexcept {
  return type() == EventType::thread_on_create ||
         type() == EventType::mutex_on_create ||
         type() == EventType::rec_mutex_on_create ||
         type() == EventType::rwlock_on_create;
}

bool Event::is_destroy_event() const noexcept {
  return type() == EventType::thread_on_join ||
         type() == EventType::mutex_on_destroy ||
         type() == EventType::rec_mutex_on_destroy ||
         type() == EventType::rwlock_on_destroy;
}

ObjectType Event::object_type() const noexcept {
  switch (type()) {
  case EventType::thread_on_create:
  case EventType::thread_on_join:
  case EventType::thread_on_detach: return ObjectType::thread;
  case EventType::mutex_on_create:
  case EventType::mutex_before_lock:
  case EventType::mutex_after_lock:
  case EventType::mutex_on_try_lock:
  case EventType::mutex_on_unlock:
  case EventType::mutex_on_destroy: return ObjectType::mutex;
  case EventType::rec_mutex_on_create:
  case EventType::rec_mutex_before_lock:
  case EventType::rec_mutex_after_lock:
  case EventType::rec_mutex_on_try_lock:
  case EventType::rec_mutex_on_unlock:
  case EventType::rec_mutex_on_destroy: return ObjectType::rec_mutex;
  case EventType::rwlock_on_create:
  case EventType::rwlock_before_rd_lock:
  case EventType::rwlock_after_rd_lock:
  case EventType::rwlock_on_rd_unlock:
  case EventType::rwlock_on_try_rd_lock:
  case EventType::rwlock_before_wr_lock:
  case EventType::rwlock_after_wr_lock:
  case EventType::rwlock_on_wr_unlock:
  case EventType::rwlock_on_try_wr_lock:
  case EventType::rwlock_on_destroy: return ObjectType::rwlock;
  }
  std::abort();
}

std::string_view Event::object_type_str(ObjectType object_type) noexcept {
  switch (object_type) {
  case ObjectType::thread: return thread_object_type;
  case ObjectType::mutex: return mutex_object_type;
  case ObjectType::rec_mutex: return rec_mutex_object_type;
  case ObjectType::rwlock: return rwlock_object_type;
  }
}

std::string_view Event::object_type_str() const noexcept {
  return object_type_str(object_type());
}

std::string_view Event::type_str() const noexcept {
  switch (type()) {
  case EventType::thread_on_create: return "thread_create";
  case EventType::thread_on_join: return "thread_join";
  case EventType::thread_on_detach: return "thread_detach";
  case EventType::mutex_on_create: return "mutex_create";
  case EventType::mutex_before_lock: return "mutex_before_lock";
  case EventType::mutex_after_lock: return "mutex_after_lock";
  case EventType::mutex_on_try_lock: return "mutex_try_lock";
  case EventType::mutex_on_unlock: return "mutex_unlock";
  case EventType::mutex_on_destroy: return "mutex_destroy";
  case EventType::rec_mutex_on_create: return "recursive_mutex_create";
  case EventType::rec_mutex_before_lock: return "recursive_mutex_before_lock";
  case EventType::rec_mutex_after_lock: return "recursive_mutex_after_lock";
  case EventType::rec_mutex_on_try_lock: return "recursive_mutex_try_lock";
  case EventType::rec_mutex_on_unlock: return "recursive_mutex_unlock";
  case EventType::rec_mutex_on_destroy: return "recursive_mutex_destroy";
  case EventType::rwlock_on_create: return "read_write_lock_create";
  case EventType::rwlock_before_rd_lock:
    return "read_write_lock_before_read_lock";
  case EventType::rwlock_after_rd_lock:
    return "read_write_lock_after_read_lock";
  case EventType::rwlock_on_rd_unlock: return "read_write_lock_read_unlock";
  case EventType::rwlock_on_try_rd_lock: return "read_write_lock_try_read_lock";
  case EventType::rwlock_before_wr_lock:
    return "read_write_lock_before_write_lock";
  case EventType::rwlock_after_wr_lock:
    return "read_write_lock_after_write_lock";
  case EventType::rwlock_on_wr_unlock: return "read_write_lock_write_unlock";
  case EventType::rwlock_on_try_wr_lock:
    return "read_write_lock_try_write_lock";
  case EventType::rwlock_on_destroy: return "read_write_lock_destroy";
  }
  std::abort();
}

Event::operator bool() const noexcept {
  return ptr != nullptr;
}

bool Event::operator==(decltype(nullptr)) const noexcept {
  return ptr == nullptr;
}

bool Event::operator!=(decltype(nullptr)) const noexcept {
  return ptr != nullptr;
}

bool Event::operator<(const Event& other) const noexcept {
  return ptr < other.ptr;
}

bool Event::operator>(const Event& other) const noexcept {
  return ptr > other.ptr;
}

bool Event::operator<=(const Event& other) const noexcept {
  return ptr <= other.ptr;
}

bool Event::operator>=(const Event& other) const noexcept {
  return ptr >= other.ptr;
}

bool Event::operator==(const Event& other) const noexcept {
  return ptr == other.ptr;
}

bool Event::operator!=(const Event& other) const noexcept {
  return ptr != other.ptr;
}

}  // namespace syan