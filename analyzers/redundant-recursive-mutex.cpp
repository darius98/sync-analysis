#include <map>
#include <optional>

#include "utils.hpp"
#include <syan_analyzer_api/syan_analyzer_api.hpp>

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

struct RedundantRecursiveMutexAnalyzer {
  static constexpr const char* name = "redundant-recursive-mutex";

  std::map<ObjectId, RecursiveMutexState> recursive_mutexes;

  void operator()(Event event) {
    if (object_type(event) != ObjectType::rec_mutex) {
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
    case SA_EV_MUTEX_AFTER_LOCK: {
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
    case SA_EV_MUTEX_ON_UNLOCK: {
      it->second.thread_owner.reset();
      break;
    }
    case SA_EV_MUTEX_ON_DESTROY: {
      recursive_mutexes.erase(it);
    }
    default: break;
    }
  }
};

SYAN_ANALYZER_CLASS(RedundantRecursiveMutexAnalyzer);
