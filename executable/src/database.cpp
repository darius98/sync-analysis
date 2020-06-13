#include "syan_analyzer_api/database.hpp"

namespace syan {

void Database::handle_event_before_analyzers(const Event& event) {
  if (event.is_create_event()) {
    active_objects.emplace(event.object(), event);
    object_names.emplace(event.object(), ++last_used_name);
  }
}

void Database::handle_event_after_analyzers(const Event& event) {
  if (event.is_destroy_event()) {
    active_objects.erase(event.object());
    object_names.erase(event.object());
  }
}

std::string Database::thread_name(ObjectId thread_id) const {
  return object_name(thread_id);
}

std::string Database::thread_name(const Event& event) const {
  return thread_name(event.thread());
}

std::string Database::object_name(const Event& event) const {
  return object_name(event.object());
}

std::string Database::object_name(ObjectId object_id) const {
  return std::to_string(object_names.at(object_id));
}

Event Database::thread_create(ObjectId thread_id) const noexcept {
  return active_objects.at(thread_id);
}

Event Database::thread_create(const Event& event) const noexcept {
  return thread_create(event.thread());
}

Event Database::object_create(const Event& event) const noexcept {
  return object_create(event.object());
}

Event Database::object_create(ObjectId object_id) const noexcept {
  return active_objects.at(object_id);
}

}  // namespace syan
