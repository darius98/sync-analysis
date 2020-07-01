#ifndef SYNC_ANALYSIS_ANALYZERS_UTILS_H_
#define SYNC_ANALYSIS_ANALYZERS_UTILS_H_

#include <syan_analyzer_api/syan_analyzer_api.hpp>

#include "../integration/shared/events.h"

namespace syan {

constexpr std::string_view thread_object_type_str = "thread";
constexpr std::string_view mutex_object_type_str = "mutex";
constexpr std::string_view rec_mutex_object_type_str = "recursive mutex";
constexpr std::string_view rwlock_object_type_str = "read-write lock";
constexpr std::string_view unknown_str = "unknown";

enum class ObjectType {
  thread,
  mutex,
  rec_mutex,
  rwlock,
  unknown,
};

inline ObjectType object_type(const Event& event) noexcept {
  if (event.type() & SA_EV_THREAD) {
    return ObjectType::thread;
  }
  if (event.type() & SA_REC_MUTEX) {
    return ObjectType::rec_mutex;
  }
  if (event.type() & SA_MUTEX) {
    if (database().object_create(event.object()).type() & SA_REC_MUTEX) {
      return ObjectType::rec_mutex;
    }
    return ObjectType::mutex;
  }
  if (event.type() & SA_RWLOCK) {
    return ObjectType::rwlock;
  }
  return ObjectType::unknown;
}

inline std::string_view object_type_str(ObjectType object_type) noexcept {
  switch (object_type) {
  case ObjectType::thread: return thread_object_type_str;
  case ObjectType::mutex: return mutex_object_type_str;
  case ObjectType::rec_mutex: return rec_mutex_object_type_str;
  case ObjectType::rwlock: return rwlock_object_type_str;
  case ObjectType::unknown: return unknown_str;
  }
}

inline std::string_view object_type_str(const Event& event) noexcept {
  return object_type_str(object_type(event));
}

std::string_view event_type_str(const Event& event) noexcept {
#define EVENT_TYPE_STR_CASE(E)                                                 \
  case SA_EV_##E: return #E;
  switch (event.type()) {
    EVENT_TYPE_STR_CASE(THREAD_ON_CREATE)
    EVENT_TYPE_STR_CASE(THREAD_ON_JOIN)
    EVENT_TYPE_STR_CASE(THREAD_ON_DETACH)
    EVENT_TYPE_STR_CASE(MUTEX_ON_CREATE)
    EVENT_TYPE_STR_CASE(MUTEX_BEFORE_LOCK)
    EVENT_TYPE_STR_CASE(MUTEX_AFTER_LOCK)
    EVENT_TYPE_STR_CASE(MUTEX_ON_TRY_LOCK)
    EVENT_TYPE_STR_CASE(MUTEX_ON_UNLOCK)
    EVENT_TYPE_STR_CASE(MUTEX_ON_DESTROY)
    EVENT_TYPE_STR_CASE(REC_MUTEX_ON_CREATE)
    EVENT_TYPE_STR_CASE(RWLOCK_ON_CREATE)
    EVENT_TYPE_STR_CASE(RWLOCK_BEFORE_RD_LOCK)
    EVENT_TYPE_STR_CASE(RWLOCK_AFTER_RD_LOCK)
    EVENT_TYPE_STR_CASE(RWLOCK_ON_TRY_RD_LOCK)
    EVENT_TYPE_STR_CASE(RWLOCK_BEFORE_WR_LOCK)
    EVENT_TYPE_STR_CASE(RWLOCK_AFTER_WR_LOCK)
    EVENT_TYPE_STR_CASE(RWLOCK_ON_TRY_WR_LOCK)
    EVENT_TYPE_STR_CASE(RWLOCK_ON_UNLOCK)
    EVENT_TYPE_STR_CASE(RWLOCK_ON_DESTROY)
  default: return unknown_str;
  }
#undef EVENT_TYPE_STR_CASE
}

}  // namespace syan

#endif
