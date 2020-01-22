#include "active_objects_db.hpp"

#include "utils.hpp"

namespace syan {

void ActiveObjectsDb::handle_event_before_checks(const EventPtr& event) {
  if (is_create_event(event)) {
    auto key = std::pair{get_object_type(event), event->addr};
    active_objects.emplace(key, event);
    object_names.emplace(key, ++last_used_name[get_object_type(event)]);
  }

  if (event->event_type == SA_EV_THREAD_ON_CREATE) {
    active_threads.emplace(event->addr, ThreadState{event, nullptr});
  }

  if (event->event_type == SA_EV_THREAD_ON_DETACH) {
    active_threads.at(event->addr).detach = event;
  }
}

void ActiveObjectsDb::handle_event_after_checks(const EventPtr& event) {
  if (is_destroy_event(event)) {
    auto key = std::pair{get_object_type(event), event->addr};
    active_objects.erase(key);
    object_names.erase(key);
  }

  if (event->event_type == SA_EV_THREAD_ON_JOIN) {
    active_threads.erase(event->addr);
  }
}

std::string ActiveObjectsDb::thread_name(ObjectId thread_id) const {
  return object_name(ObjectType::thread, thread_id);
}

std::string ActiveObjectsDb::thread_name(const Event& event) const {
  return thread_name(event.thread_id);
}

std::string ActiveObjectsDb::thread_name(const EventPtr& event) const {
  return thread_name(*event);
}

std::string ActiveObjectsDb::object_name(const Event& event) const {
  return object_name(get_object_type(event), event.addr);
}

std::string ActiveObjectsDb::object_name(const EventPtr& event) const {
  return object_name(*event);
}

std::string ActiveObjectsDb::object_name(ObjectType object_type,
                                         ObjectId object_id) const {
  return std::string{get_object_type_str(object_type)} + " " +
         std::to_string(object_names.at({object_type, object_id}));
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
  return active_objects.at(std::pair{get_object_type(event), event.addr});
}

EventPtr ActiveObjectsDb::object_create(const EventPtr& event) const noexcept {
  return object_create(*event);
}

} // namespace syan
