#include "active_objects_db.hpp"

#include <lib/src/event_type.h>

namespace syan {

void ActiveObjectsDb::insert(const Event& event) {
  switch (event.event_type) {
  case SA_EV_THREAD_ON_CREATE: {
    active_threads.emplace(event.addr, event);
    active_threads_by_id.emplace(event.thread_id, event);
    break;
  }
  case SA_EV_THREAD_ON_JOIN: {
    auto create_event = active_threads.at(event.addr);
    active_threads.erase(create_event.addr);
    active_threads_by_id.erase(create_event.thread_id);
    break;
  }
  case SA_EV_THREAD_ON_DETACH: {
    auto create_event = active_threads.at(event.addr);
    detached_threads.emplace(create_event.thread_id, event);
    break;
  }
  case SA_EV_MUTEX_ON_CREATE: {
    active_mutexes.emplace(event.addr, event);
    break;
  }
  case SA_EV_MUTEX_ON_DESTROY: {
    active_mutexes.erase(event.addr);
    break;
  }
  case SA_EV_REC_MUTEX_ON_CREATE: {
    active_rec_mutexes.emplace(event.addr, event);
    break;
  }
  case SA_EV_REC_MUTEX_ON_DESTROY: {
    active_rec_mutexes.erase(event.addr);
    break;
  }
  case SA_EV_RWLOCK_ON_CREATE: {
    active_rwlocks.emplace(event.addr, event);
    break;
  }
  case SA_EV_RWLOCK_ON_DESTROY: {
    active_rwlocks.erase(event.addr);
    break;
  }
  }
}

namespace {

template <class Map, class Id>
std::optional<Event> find_event_in_map(const Map& map, Id id) noexcept {
  auto it = map.find(id);
  if (it == map.end()) {
    return std::nullopt;
  }
  return it->second;
}

} // namespace

std::optional<Event>
ActiveObjectsDb::get_thread_create_event_by_thread_id(ThreadId thread_id) const
    noexcept {
  return find_event_in_map(active_threads_by_id, thread_id);
}

std::optional<Event>
ActiveObjectsDb::get_thread_detach_event_by_thread_id(ThreadId thread_id) const
    noexcept {
  auto create_event = find_event_in_map(active_threads_by_id, thread_id);
  if (!create_event.has_value()) {
    return std::nullopt;
  }
  return find_event_in_map(detached_threads, create_event.value().thread_id);
}

std::optional<Event>
ActiveObjectsDb::get_mutex_create_event(ObjectId mutex_id) const noexcept {
  return find_event_in_map(active_mutexes, mutex_id);
}

std::optional<Event>
ActiveObjectsDb::get_rec_mutex_create_event(ObjectId rec_mutex_id) const
    noexcept {
  return find_event_in_map(active_rec_mutexes, rec_mutex_id);
}

std::optional<Event>
ActiveObjectsDb::get_rwlock_create_event(ObjectId rwlock_id) const noexcept {
  return find_event_in_map(active_rwlocks, rwlock_id);
}

} // namespace syan
