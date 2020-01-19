#include "active_objects_db.hpp"

#include <iomanip>
#include <sstream>

#include "utils.hpp"

namespace syan {

void ActiveObjectsDb::insert(EventPtr event) {
  switch (event->event_type) {
  case SA_EV_THREAD_ON_CREATE: {
    active_threads.emplace(event->addr, ThreadState{std::move(event), nullptr});
    break;
  }
  case SA_EV_THREAD_ON_JOIN: {
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

std::string ActiveObjectsDb::thread_name(ObjectId thread_id) const {
  return object_name(thread_object_type, thread_id);
}

std::string ActiveObjectsDb::mutex_name(ObjectId mutex_id) const {
  return object_name(mutex_object_type, mutex_id);
}

std::string ActiveObjectsDb::object_name(const Event& event) const {
  return object_name(get_object_type(event), event.addr);
}

std::string ActiveObjectsDb::object_name(std::string_view object_type,
                                         ObjectId object_id) const {
  std::stringstream builder;
  builder << object_type << " " << std::hex << std::setfill('0')
          << std::setw(16) << object_id;
  return builder.str();
}

EventPtr ActiveObjectsDb::thread_create(ObjectId thread_id) const noexcept {
  return active_threads.at(thread_id).create;
}

EventPtr ActiveObjectsDb::thread_detach(ObjectId thread_id) const noexcept {
  return active_threads.at(thread_id).detach;
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
