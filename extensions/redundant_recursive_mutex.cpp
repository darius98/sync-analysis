#include <map>
#include <optional>

#include <syan_extension_api/syan_extension_api.hpp>

using namespace syan;

struct RecursiveMutexState {
  Event create_event;
  bool was_ever_locked{false};
  std::optional<ObjectId> thread_owner;

  explicit RecursiveMutexState(Event create_event)
      : create_event(std::move(create_event)) {}
};

std::map<ObjectId, RecursiveMutexState> recursive_mutexes;

SYAN_EXT_API const char* syan_extension = "redundant-recursive-mutex";

SYAN_EXT_API void syan_extension_on_event() {
  auto event = current_event();
  if (event.object_type() != ObjectType::rec_mutex) {
    return;
  }
  if (event.type() == EventType::rec_mutex_on_create) {
    recursive_mutexes.emplace(event.object(), event);
    return;
  }

  auto it = recursive_mutexes.find(event.object());
  if (it == recursive_mutexes.end()) {
    return;
  }

  switch (event.type()) {
  case EventType::rec_mutex_after_lock: {
    it->second.was_ever_locked = true;
    if (it->second.thread_owner.has_value()) {
      // Found a legitimate use of a recursive mutex. Backing off.
      recursive_mutexes.erase(it);
      return;
    }
    it->second.thread_owner.emplace(event.thread());
    break;
  }
  case EventType::rec_mutex_on_unlock: {
    it->second.thread_owner.reset();
    break;
  }
  case EventType::rec_mutex_on_destroy: {
    // Don't report if the mutex is unused instead.
    if (it->second.was_ever_locked) {
      // Found a recursive mutex that is only used as a mutex.
      auto report = create_report();
      report.set_level(Report::Level::warning);
      report.set_description(
          "Recursive mutex is redundant. Can be replaced with simple mutex.");
      report.add_section("Recursive mutex created here",
                         it->second.create_event);
    }
    recursive_mutexes.erase(it);
  }
  default: break;
  }
}

SYAN_EXT_API void syan_extension_shut_down() {
  for (const auto& [rec_mutex_id, state] : recursive_mutexes) {
    if (state.was_ever_locked) {
      auto report = create_report();
      report.set_level(Report::Level::warning);
      report.set_description(
          "Recursive mutex is redundant. Can be replaced with simple mutex.");
      report.add_section("Recursive mutex created here", state.create_event);
    }
  }
}
