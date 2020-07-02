#include <algorithm>
#include <map>
#include <set>

#include <syan_analyzer_api/syan_analyzer_api.hpp>

#include "utils.hpp"

using namespace syan;

struct LockState {
  ObjectType lock_type;
  ObjectId lock_id;
  bool was_ever_locked{false};
  std::set<Event> shadows;

  explicit LockState(ObjectType lock_type, ObjectId lock_id)
      : lock_type(lock_type), lock_id(lock_id) {}

  ~LockState() {
    if (!was_ever_locked || shadows.empty()) {
      return;
    }
    auto shadow = *shadows.begin();
    auto report = create_report();
    report.set_level(Report::Level::warning);
    report.set_description(
        "Every lock event on " + database().object_name(lock_id) +
        " is shadowed by a write lock on " + database().object_name(shadow));
    report.add_section("Note: " + database().object_name(lock_id) +
                           " created here:",
                       database().object_create(lock_id));
    report.add_section(
        "Note: " + database().object_name(shadow) + " created here:", shadow);
  }
};

struct LockShadowAnalyzer {
  static constexpr const char* name = "lock-shadow";

  std::map<ObjectId, std::multiset<Event>> thread_locks;
  std::map<ObjectId, LockState> locks;

  void operator()(Event event) {
    auto& current_thread_locks = thread_locks[event.thread()];

    if (event.type() == SA_EV_MUTEX_ON_CREATE ||
        event.type() == SA_EV_REC_MUTEX_ON_CREATE ||
        event.type() == SA_EV_RWLOCK_ON_CREATE) {
      locks.emplace(event.object(),
                    LockState{object_type(event), event.object()});
      return;
    }

    if (event.type() == SA_EV_MUTEX_ON_UNLOCK ||
        event.type() == SA_EV_RWLOCK_ON_UNLOCK) {
      auto first_occurrence =
          current_thread_locks.find(database().object_create(event));
      if (first_occurrence != current_thread_locks.end()) {
        current_thread_locks.erase(first_occurrence);
      }
      return;
    }

    auto it = locks.find(event.object());

    if (event.type() == SA_EV_MUTEX_AFTER_LOCK ||
        event.type() == SA_EV_RWLOCK_AFTER_WR_LOCK ||
        event.type() == SA_EV_RWLOCK_AFTER_RD_LOCK) {
      if (it != locks.end()) {
        if (!it->second.was_ever_locked) {
          it->second.shadows.insert(current_thread_locks.begin(),
                                    current_thread_locks.end());
        } else {
          std::set<Event> intersection;
          std::set_intersection(
              it->second.shadows.begin(), it->second.shadows.end(),
              current_thread_locks.begin(), current_thread_locks.end(),
              std::inserter(intersection, intersection.begin()));
          it->second.shadows = intersection;
        }
        it->second.was_ever_locked = true;
        if (it->second.shadows.empty()) {
          locks.erase(it);
        }
      }
      if (event.type() != SA_EV_RWLOCK_AFTER_RD_LOCK) {
        current_thread_locks.insert(database().object_create(event));
      }
    }

    if (event.type() == SA_EV_MUTEX_ON_DESTROY ||
        event.type() == SA_EV_RWLOCK_ON_DESTROY) {
      if (it != locks.end()) {
        locks.erase(it);
      }
    }
  }
};

SYAN_ANALYZER_CLASS(LockShadowAnalyzer);
