#include "event.hpp"

namespace syan {

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
  return type() == SA_EV_THREAD_ON_CREATE || type() == SA_EV_MUTEX_ON_CREATE ||
         type() == SA_EV_REC_MUTEX_ON_CREATE ||
         type() == SA_EV_RWLOCK_ON_CREATE;
}

bool Event::is_destroy_event() const noexcept {
  return type() == SA_EV_THREAD_ON_JOIN || type() == SA_EV_MUTEX_ON_DESTROY ||
         type() == SA_EV_REC_MUTEX_ON_DESTROY ||
         type() == SA_EV_RWLOCK_ON_DESTROY;
}

ObjectType Event::object_type() const noexcept {
  switch (type()) {
  case SA_EV_THREAD_ON_CREATE:
  case SA_EV_THREAD_ON_JOIN:
  case SA_EV_THREAD_ON_DETACH: return ObjectType::thread;
  case SA_EV_MUTEX_ON_CREATE:
  case SA_EV_MUTEX_BEFORE_LOCK:
  case SA_EV_MUTEX_AFTER_LOCK:
  case SA_EV_MUTEX_ON_TRY_LOCK:
  case SA_EV_MUTEX_ON_UNLOCK:
  case SA_EV_MUTEX_ON_DESTROY: return ObjectType::mutex;
  case SA_EV_REC_MUTEX_ON_CREATE:
  case SA_EV_REC_MUTEX_BEFORE_LOCK:
  case SA_EV_REC_MUTEX_AFTER_LOCK:
  case SA_EV_REC_MUTEX_ON_TRY_LOCK:
  case SA_EV_REC_MUTEX_ON_UNLOCK:
  case SA_EV_REC_MUTEX_ON_DESTROY: return ObjectType::rec_mutex;
  case SA_EV_RWLOCK_ON_CREATE:
  case SA_EV_RWLOCK_BEFORE_RD_LOCK:
  case SA_EV_RWLOCK_AFTER_RD_LOCK:
  case SA_EV_RWLOCK_ON_RD_UNLOCK:
  case SA_EV_RWLOCK_ON_TRY_RD_LOCK:
  case SA_EV_RWLOCK_BEFORE_WR_LOCK:
  case SA_EV_RWLOCK_AFTER_WR_LOCK:
  case SA_EV_RWLOCK_ON_WR_UNLOCK:
  case SA_EV_RWLOCK_ON_TRY_WR_LOCK:
  case SA_EV_RWLOCK_ON_DESTROY: return ObjectType::rwlock;
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
  case SA_EV_THREAD_ON_CREATE: return "thread_create";
  case SA_EV_THREAD_ON_JOIN: return "thread_join";
  case SA_EV_THREAD_ON_DETACH: return "thread_detach";
  case SA_EV_MUTEX_ON_CREATE: return "mutex_create";
  case SA_EV_MUTEX_BEFORE_LOCK: return "mutex_before_lock";
  case SA_EV_MUTEX_AFTER_LOCK: return "mutex_after_lock";
  case SA_EV_MUTEX_ON_TRY_LOCK: return "mutex_try_lock";
  case SA_EV_MUTEX_ON_UNLOCK: return "mutex_unlock";
  case SA_EV_MUTEX_ON_DESTROY: return "mutex_destroy";
  case SA_EV_REC_MUTEX_ON_CREATE: return "recursive_mutex_create";
  case SA_EV_REC_MUTEX_BEFORE_LOCK: return "recursive_mutex_before_lock";
  case SA_EV_REC_MUTEX_AFTER_LOCK: return "recursive_mutex_after_lock";
  case SA_EV_REC_MUTEX_ON_TRY_LOCK: return "recursive_mutex_try_lock";
  case SA_EV_REC_MUTEX_ON_UNLOCK: return "recursive_mutex_unlock";
  case SA_EV_REC_MUTEX_ON_DESTROY: return "recursive_mutex_destroy";
  case SA_EV_RWLOCK_ON_CREATE: return "read_write_lock_create";
  case SA_EV_RWLOCK_BEFORE_RD_LOCK: return "read_write_lock_before_read_lock";
  case SA_EV_RWLOCK_AFTER_RD_LOCK: return "read_write_lock_after_read_lock";
  case SA_EV_RWLOCK_ON_RD_UNLOCK: return "read_write_lock_read_unlock";
  case SA_EV_RWLOCK_ON_TRY_RD_LOCK: return "read_write_lock_try_read_lock";
  case SA_EV_RWLOCK_BEFORE_WR_LOCK: return "read_write_lock_before_write_lock";
  case SA_EV_RWLOCK_AFTER_WR_LOCK: return "read_write_lock_after_write_lock";
  case SA_EV_RWLOCK_ON_WR_UNLOCK: return "read_write_lock_write_unlock";
  case SA_EV_RWLOCK_ON_TRY_WR_LOCK: return "read_write_lock_try_write_lock";
  case SA_EV_RWLOCK_ON_DESTROY: return "read_write_lock_destroy";
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