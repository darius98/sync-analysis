#include <map>
#include <set>

#include <syan_extension_api/syan_extension_api.hpp>

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
        "Every lock event on " + database().object_name(lock_type, lock_id) +
        " is shadowed by a write lock on " + database().object_name(shadow));
    report.add_section("Note: " + database().object_name(lock_type, lock_id) +
                           " created here:",
                       database().object_create(lock_type, lock_id));
    report.add_section(
        "Note: " + database().object_name(shadow) + " created here:", shadow);
  }
};

struct LockShadowExtension {
  static constexpr const char* name = "lock-shadow";

  std::map<ObjectId, std::multiset<Event>> thread_locks;
  std::map<ObjectId, LockState> locks;

  void operator()(Event event) {
    auto& current_thread_locks = thread_locks[event.thread()];

    if (event.type() == EventType::mutex_on_create ||
        event.type() == EventType::rec_mutex_on_create ||
        event.type() == EventType::rwlock_on_create) {
      locks.emplace(event.object(),
                    LockState{event.object_type(), event.object()});
      return;
    }

    if (event.type() == EventType::mutex_on_unlock ||
        event.type() == EventType::rec_mutex_on_unlock ||
        event.type() == EventType::rwlock_on_wr_unlock) {
      auto first_occurrence =
          current_thread_locks.find(database().object_create(event));
      if (first_occurrence != current_thread_locks.end()) {
        current_thread_locks.erase(first_occurrence);
      }
      return;
    }

    auto it = locks.find(event.object());

    if (event.type() == EventType::mutex_after_lock ||
        event.type() == EventType::rec_mutex_after_lock ||
        event.type() == EventType::rwlock_after_wr_lock ||
        event.type() == EventType::rwlock_after_rd_lock) {
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
      if (event.type() != EventType::rwlock_after_rd_lock) {
        current_thread_locks.insert(database().object_create(event));
      }
    }

    if (event.type() == EventType::mutex_on_destroy ||
        event.type() == EventType::rec_mutex_on_destroy ||
        event.type() == EventType::rwlock_on_destroy) {
      if (it != locks.end()) {
        locks.erase(it);
      }
    }
  }
};

SYAN_EXTENSION_SET_CLASS(LockShadowExtension);
