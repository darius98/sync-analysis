#include <map>
#include <optional>

#include <syan_extension_api/syan_extension_api.hpp>

using namespace syan;

struct RecursiveMutexState {
  Event create_event;
  bool was_ever_locked{false};
  bool was_recursively_locked{false};
  std::optional<ObjectId> thread_owner;

  explicit RecursiveMutexState(Event create_event)
      : create_event(std::move(create_event)) {}

  ~RecursiveMutexState() {
    // Don't report if the mutex is unused instead.
    if (was_ever_locked && !was_recursively_locked) {
      auto report = create_report();
      report.set_level(Report::Level::warning);
      report.set_description(
          "Recursive mutex is redundant. Can be replaced with simple mutex.");
      report.add_section("Recursive mutex created here", create_event);
    }
  }
};

struct RedundantRecursiveMutexExtension {
  static constexpr const char* name = "redundant-recursive-mutex";

  std::map<ObjectId, RecursiveMutexState> recursive_mutexes;

  void operator()(Event event) {
    if (event.object_type() != ObjectType::rec_mutex) {
      return;
    }
    if (event.is_create_event()) {
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
        it->second.was_recursively_locked = true;
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
      recursive_mutexes.erase(it);
    }
    default: break;
    }
  }
};

SYAN_EXTENSION_SET_CLASS(RedundantRecursiveMutexExtension);
