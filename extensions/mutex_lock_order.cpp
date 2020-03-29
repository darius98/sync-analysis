#include <map>
#include <set>

#include <syan_extension_api/syan_extension_api.hpp>

using namespace syan;

std::map<std::pair<ObjectId, ObjectId>, std::pair<Event, Event>>
    ordered_objects;
std::map<ObjectId, std::set<Event>> thread_locked_objects;

SYAN_EXT_API const char* syan_extension = "mutex_lock_order";

SYAN_EXT_API void syan_extension_on_event() {
  auto event = current_event();
  switch (event.type()) {
  case EventType::mutex_before_lock: {
    auto& locked_on_thread = thread_locked_objects[event.thread()];
    for (const auto& prev_lock_event : locked_on_thread) {
      auto it =
          ordered_objects.find({event.object(), prev_lock_event.object()});
      if (it != ordered_objects.end()) {
        const auto& [mtx1_lock, mtx2_lock] = it->second;

        auto report = create_report();
        report.set_level(Report::error);
        report.set_description("Mutex lock order inconsistency found");
        report.add_section(database().thread_name(mtx1_lock) + " locked " +
                               database().object_name(mtx1_lock) +
                               " first here:",
                           mtx1_lock);
        report.add_section(database().thread_name(mtx2_lock) + " locked " +
                               database().object_name(mtx2_lock) +
                               " second here:",
                           mtx2_lock);
        report.add_section(
            database().thread_name(prev_lock_event) + " locked " +
                database().object_name(prev_lock_event) + " first here:",
            prev_lock_event);
        report.add_section(database().thread_name(event) +
                               " attempted to lock " +
                               database().object_name(event) + " second here:",
                           event);
      }
    }
    break;
  }
  case EventType::mutex_after_lock: {
    thread_locked_objects[event.thread()].insert(event);
    auto& locked_on_thread = thread_locked_objects[event.thread()];
    for (const auto& prev_lock_event : locked_on_thread) {
      ordered_objects.try_emplace(
          std::pair{prev_lock_event.object(), event.object()},
          std::pair{prev_lock_event, event});
    }
    break;
  }
  case EventType::mutex_on_unlock: {
    thread_locked_objects[event.thread()].erase(event);
    break;
  }
  default: break;
  }
}