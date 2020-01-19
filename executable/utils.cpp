#include "utils.hpp"

#include <cstdlib>

namespace syan {

bool is_create_event(const Event& event) noexcept {
  return event.event_type == SA_EV_THREAD_ON_CREATE ||
         event.event_type == SA_EV_MUTEX_ON_CREATE ||
         event.event_type == SA_EV_REC_MUTEX_ON_CREATE ||
         event.event_type == SA_EV_RWLOCK_ON_CREATE;
}

bool is_create_event(const EventPtr& event) noexcept {
  return is_create_event(*event);
}

bool is_destroy_event(const Event& event) noexcept {
  return event.event_type == SA_EV_THREAD_ON_JOIN ||
         event.event_type == SA_EV_THREAD_ON_DETACH ||
         event.event_type == SA_EV_MUTEX_ON_DESTROY ||
         event.event_type == SA_EV_REC_MUTEX_ON_DESTROY ||
         event.event_type == SA_EV_RWLOCK_ON_DESTROY;
}

bool is_destroy_event(const EventPtr& event) noexcept {
  return is_destroy_event(*event);
}

ObjectType get_object_type(const Event& event) noexcept {
  switch (event.event_type) {
  case SA_EV_THREAD_ON_CREATE:
  case SA_EV_THREAD_ON_JOIN:
  case SA_EV_THREAD_ON_DETACH:
    return ObjectType::thread;
  case SA_EV_MUTEX_ON_CREATE:
  case SA_EV_MUTEX_BEFORE_LOCK:
  case SA_EV_MUTEX_AFTER_LOCK:
  case SA_EV_MUTEX_ON_TRY_LOCK:
  case SA_EV_MUTEX_ON_UNLOCK:
  case SA_EV_MUTEX_ON_DESTROY:
    return ObjectType::mutex;
  case SA_EV_REC_MUTEX_ON_CREATE:
  case SA_EV_REC_MUTEX_BEFORE_LOCK:
  case SA_EV_REC_MUTEX_AFTER_LOCK:
  case SA_EV_REC_MUTEX_ON_TRY_LOCK:
  case SA_EV_REC_MUTEX_ON_UNLOCK:
  case SA_EV_REC_MUTEX_ON_DESTROY:
    return ObjectType::rec_mutex;
  case SA_EV_RWLOCK_ON_CREATE:
  case SA_EV_RWLOCK_BEFORE_RD_LOCK:
  case SA_EV_RWLOCK_AFTER_RD_LOCK:
  case SA_EV_RWLOCK_ON_RD_UNLOCK:
  case SA_EV_RWLOCK_ON_TRY_RD_LOCK:
  case SA_EV_RWLOCK_BEFORE_WR_LOCK:
  case SA_EV_RWLOCK_AFTER_WR_LOCK:
  case SA_EV_RWLOCK_ON_WR_UNLOCK:
  case SA_EV_RWLOCK_ON_TRY_WR_LOCK:
  case SA_EV_RWLOCK_ON_DESTROY:
    return ObjectType::rwlock;
  }
  std::abort();
}

ObjectType get_object_type(const EventPtr& event) noexcept {
  return get_object_type(*event);
}

std::string_view get_object_type_str(ObjectType object_type) noexcept {
  switch (object_type) {
  case ObjectType::thread:
    return thread_object_type;
  case ObjectType::mutex:
    return mutex_object_type;
  case ObjectType::rec_mutex:
    return rec_mutex_object_type;
  case ObjectType::rwlock:
    return rwlock_object_type;
  }
}

std::string_view get_object_type_str(const Event& event) noexcept {
  return get_object_type_str(get_object_type(event));
}

std::string_view get_object_type_str(const EventPtr& event) noexcept {
  return get_object_type_str(*event);
}

std::string_view get_event_type_str(const Event& event) noexcept {
  switch (event.event_type) {
  case SA_EV_THREAD_ON_CREATE:
    return "thread_create";
  case SA_EV_THREAD_ON_JOIN:
    return "thread_join";
  case SA_EV_THREAD_ON_DETACH:
    return "thread_detach";
  case SA_EV_MUTEX_ON_CREATE:
    return "mutex_create";
  case SA_EV_MUTEX_BEFORE_LOCK:
    return "mutex_before_lock";
  case SA_EV_MUTEX_AFTER_LOCK:
    return "mutex_after_lock";
  case SA_EV_MUTEX_ON_TRY_LOCK:
    return "mutex_try_lock";
  case SA_EV_MUTEX_ON_UNLOCK:
    return "mutex_unlock";
  case SA_EV_MUTEX_ON_DESTROY:
    return "mutex_destroy";
  case SA_EV_REC_MUTEX_ON_CREATE:
    return "recursive_mutex_create";
  case SA_EV_REC_MUTEX_BEFORE_LOCK:
    return "recursive_mutex_before_lock";
  case SA_EV_REC_MUTEX_AFTER_LOCK:
    return "recursive_mutex_after_lock";
  case SA_EV_REC_MUTEX_ON_TRY_LOCK:
    return "recursive_mutex_try_lock";
  case SA_EV_REC_MUTEX_ON_UNLOCK:
    return "recursive_mutex_unlock";
  case SA_EV_REC_MUTEX_ON_DESTROY:
    return "recursive_mutex_destroy";
  case SA_EV_RWLOCK_ON_CREATE:
    return "read_write_lock_create";
  case SA_EV_RWLOCK_BEFORE_RD_LOCK:
    return "read_write_lock_before_read_lock";
  case SA_EV_RWLOCK_AFTER_RD_LOCK:
    return "read_write_lock_after_read_lock";
  case SA_EV_RWLOCK_ON_RD_UNLOCK:
    return "read_write_lock_read_unlock";
  case SA_EV_RWLOCK_ON_TRY_RD_LOCK:
    return "read_write_lock_try_read_lock";
  case SA_EV_RWLOCK_BEFORE_WR_LOCK:
    return "read_write_lock_before_write_lock";
  case SA_EV_RWLOCK_AFTER_WR_LOCK:
    return "read_write_lock_after_write_lock";
  case SA_EV_RWLOCK_ON_WR_UNLOCK:
    return "read_write_lock_write_unlock";
  case SA_EV_RWLOCK_ON_TRY_WR_LOCK:
    return "read_write_lock_try_write_lock";
  case SA_EV_RWLOCK_ON_DESTROY:
    return "read_write_lock_destroy";
  }
  std::abort();
}

std::string_view get_event_type_str(const EventPtr& event) noexcept {
  return get_event_type_str(*event);
}

} // namespace syan
