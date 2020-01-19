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

std::string ActiveObjectsDb::thread_name(const Event& event) const {
  return thread_name(event.thread_id);
}

std::string ActiveObjectsDb::thread_name(const EventPtr& event) const {
  return thread_name(*event);
}

std::string ActiveObjectsDb::object_name(const Event& event) const {
  return object_name(get_object_type_str(event), event.addr);
}

std::string ActiveObjectsDb::object_name(const EventPtr& event) const {
  return object_name(*event);
}

std::string ActiveObjectsDb::object_name(std::string_view object_type,
                                         ObjectId object_id) const {
  std::stringstream builder;
  builder << object_type << " " << std::hex << std::setfill('0')
          << std::setw(16) << object_id << std::dec;
  return builder.str();
}

EventPtr ActiveObjectsDb::thread_create(ObjectId thread_id) const noexcept {
  return active_threads.at(thread_id).create;
}

EventPtr ActiveObjectsDb::thread_create(const Event& event) const noexcept {
  return thread_create(event.thread_id);
}

EventPtr ActiveObjectsDb::thread_create(const EventPtr& event) const noexcept {
  return thread_create(*event);
}

EventPtr ActiveObjectsDb::thread_detach(ObjectId thread_id) const noexcept {
  return active_threads.at(thread_id).detach;
}

EventPtr ActiveObjectsDb::thread_detach(const Event& event) const noexcept {
  return thread_detach(event.thread_id);
}

EventPtr ActiveObjectsDb::thread_detach(const EventPtr& event) const noexcept {
  return thread_detach(*event);
}

EventPtr ActiveObjectsDb::object_create(const Event& event) const noexcept {
  switch (get_object_type(event)) {
  case ObjectType::thread:
    return active_threads.at(event.addr).create;
  case ObjectType::mutex:
    return active_mutexes.at(event.addr);
  case ObjectType::rec_mutex:
    return active_rec_mutexes.at(event.addr);
  case ObjectType::rwlock:
    return active_rwlocks.at(event.addr);
  }
}

EventPtr ActiveObjectsDb::object_create(const EventPtr& event) const noexcept {
  return object_create(*event);
}

} // namespace syan
