#include "active_objects_db.hpp"

namespace syan {

void ActiveObjectsDb::insert(EventPtr event) {
  switch (event->event_type) {
  case SA_EV_THREAD_ON_CREATE: {
    active_threads_id_to_addr.emplace(event->thread_id, event->addr);
    active_threads.emplace(event->addr, ThreadState{std::move(event), nullptr});
    break;
  }
  case SA_EV_THREAD_ON_JOIN: {
    const auto& thread_state = active_threads.at(event->addr);
    active_threads_id_to_addr.erase(thread_state.create->thread_id);
    active_threads.erase(event->addr);
    break;
  }
  case SA_EV_THREAD_ON_DETACH: {
    auto& thread_state = active_threads.at(event->addr);
    thread_state.detach = std::move(event);
    break;
  }
  case SA_EV_MUTEX_ON_CREATE: {
    active_mutexes.emplace(event->addr, std::move(event));
    break;
  }
  case SA_EV_MUTEX_ON_DESTROY: {
    active_mutexes.erase(event->addr);
    break;
  }
  case SA_EV_REC_MUTEX_ON_CREATE: {
    active_rec_mutexes.emplace(event->addr, std::move(event));
    break;
  }
  case SA_EV_REC_MUTEX_ON_DESTROY: {
    active_rec_mutexes.erase(event->addr);
    break;
  }
  case SA_EV_RWLOCK_ON_CREATE: {
    active_rwlocks.emplace(event->addr, std::move(event));
    break;
  }
  case SA_EV_RWLOCK_ON_DESTROY: {
    active_rwlocks.erase(event->addr);
    break;
  }
  }
}

std::string ActiveObjectsDb::thread_name(ThreadId thread_id) const {
  return std::to_string(thread_id);
}

std::string ActiveObjectsDb::mutex_name(ObjectId mutex_id) const {
  return std::to_string(mutex_id);
}

EventPtr ActiveObjectsDb::thread_create(ThreadId thread_id) const noexcept {
  const auto& thread_addr = active_threads_id_to_addr.at(thread_id);
  const auto& thread_state = active_threads.at(thread_addr);
  return thread_state.create;
}

EventPtr ActiveObjectsDb::thread_detach(ThreadId thread_id) const noexcept {
  const auto& thread_addr = active_threads_id_to_addr.at(thread_id);
  const auto& thread_state = active_threads.at(thread_addr);
  return thread_state.detach;
}

namespace {

template <class Map, class Id>
EventPtr find_event_in_map(const Map& map, Id id) noexcept {
  auto it = map.find(id);
  if (it == map.end()) {
    return nullptr;
  }
  return it->second;
}

} // namespace

EventPtr ActiveObjectsDb::mutex_create(ObjectId mutex_id) const noexcept {
  return find_event_in_map(active_mutexes, mutex_id);
}

EventPtr ActiveObjectsDb::rec_mutex_create(ObjectId rec_mutex_id) const
    noexcept {
  return find_event_in_map(active_rec_mutexes, rec_mutex_id);
}

EventPtr ActiveObjectsDb::rwlock_create(ObjectId rwlock_id) const noexcept {
  return find_event_in_map(active_rwlocks, rwlock_id);
}

} // namespace syan
