#include "active_objects_db.hpp"

namespace syan {

void ActiveObjectsDb::handle_event_before_checks(const Event& event) {
  if (event.is_create_event()) {
    auto key = std::pair{event.object_type(), event.object()};
    active_objects.emplace(key, event);
    object_names.emplace(key, ++last_used_name[event.object_type()]);
  }
}

void ActiveObjectsDb::handle_event_after_checks(const Event& event) {
  if (event.is_destroy_event()) {
    auto key = std::pair{event.object_type(), event.object()};
    active_objects.erase(key);
    object_names.erase(key);
  }
}

std::string ActiveObjectsDb::thread_name(ObjectId thread_id) const {
  return object_name(ObjectType::thread, thread_id);
}

std::string ActiveObjectsDb::thread_name(const Event& event) const {
  return thread_name(event.thread());
}

std::string ActiveObjectsDb::object_name(const Event& event) const {
  return object_name(event.object_type(), event.object());
}

std::string ActiveObjectsDb::object_name(ObjectType object_type,
                                         ObjectId object_id) const {
  return std::string{Event::object_type_str(object_type)} + " " +
         std::to_string(object_names.at({object_type, object_id}));
}

Event ActiveObjectsDb::thread_create(ObjectId thread_id) const noexcept {
  return active_objects.at({ObjectType::thread, thread_id});
}

Event ActiveObjectsDb::thread_create(const Event& event) const noexcept {
  return thread_create(event.thread());
}

Event ActiveObjectsDb::object_create(const Event& event) const noexcept {
  return active_objects.at(std::pair{event.object_type(), event.object()});
}

}  // namespace syan
