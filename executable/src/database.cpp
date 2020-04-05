#include "syan_extension_api/database.hpp"

namespace syan {

void Database::handle_event_before_extensions(const Event& event) {
  if (event.is_create_event()) {
    auto key = std::pair{event.object_type(), event.object()};
    active_objects.emplace(key, event);
    object_names.emplace(key, ++last_used_name[event.object_type()]);
  }
}

void Database::handle_event_after_extensions(const Event& event) {
  if (event.is_destroy_event()) {
    auto key = std::pair{event.object_type(), event.object()};
    active_objects.erase(key);
    object_names.erase(key);
  }
}

std::string Database::thread_name(ObjectId thread_id) const {
  return object_name(ObjectType::thread, thread_id);
}

std::string Database::thread_name(const Event& event) const {
  return thread_name(event.thread());
}

std::string Database::object_name(const Event& event) const {
  return object_name(event.object_type(), event.object());
}

std::string Database::object_name(ObjectType object_type,
                                  ObjectId object_id) const {
  return std::string{Event::object_type_str(object_type)} + " " +
         std::to_string(object_names.at({object_type, object_id}));
}

Event Database::thread_create(ObjectId thread_id) const noexcept {
  return active_objects.at({ObjectType::thread, thread_id});
}

Event Database::thread_create(const Event& event) const noexcept {
  return thread_create(event.thread());
}

Event Database::object_create(const Event& event) const noexcept {
  return object_create(event.object_type(), event.object());
}

Event Database::object_create(ObjectType object_type, ObjectId object_id) const
    noexcept {
  return active_objects.at(std::pair{object_type, object_id});
}

}  // namespace syan
