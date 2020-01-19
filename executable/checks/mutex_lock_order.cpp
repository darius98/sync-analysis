#include <map>
#include <set>

#include <executable/check.hpp>

using namespace syan;

class MutexLockOrderCheck : public Check {
public:
  void on_event(const Environment& env, EventPtr event) final {
    switch (event->event_type) {
    case SA_EV_MUTEX_BEFORE_LOCK: {
      auto& locked_on_thread = thread_locked_objects[event->thread_id];
      for (const auto& prev_lock_event : locked_on_thread) {
        auto it = ordered_objects.find({event->addr, prev_lock_event->addr});
        if (it != ordered_objects.end()) {
          const auto& [mtx1_lock, mtx2_lock] = it->second;

          auto report = env.create_report(
              Report::error, 1, "Mutex lock order inconsistency found");

          report.add_section(env.db().thread_name(mtx1_lock) + " locked " +
                                 env.db().object_name(mtx1_lock) +
                                 " first here:",
                             mtx1_lock);
          report.add_section(env.db().thread_name(mtx2_lock) + " locked " +
                                 env.db().object_name(mtx2_lock) +
                                 " second here:",
                             mtx2_lock);

          report.add_section(
              env.db().thread_name(prev_lock_event) + " locked " +
                  env.db().object_name(prev_lock_event) + " first here:",
              prev_lock_event);

          report.add_section(env.db().thread_name(event) +
                                 " attempted to lock " +
                                 env.db().object_name(event) + " second here:",
                             event);

          report.send();
        }
      }
      break;
    }
    case SA_EV_MUTEX_AFTER_LOCK: {
      thread_locked_objects[event->thread_id].insert(event);
      auto& locked_on_thread = thread_locked_objects[event->thread_id];
      for (const auto& prev_lock_event : locked_on_thread) {
        ordered_objects.try_emplace(
            std::pair{prev_lock_event->addr, event->addr},
            std::pair{prev_lock_event, event});
      }
      break;
    }
    case SA_EV_MUTEX_ON_UNLOCK: {
      thread_locked_objects[event->thread_id].erase(event);
      break;
    }
    default:
      break;
    }
  }

private:
  std::map<std::pair<ObjectId, ObjectId>, std::pair<EventPtr, EventPtr>>
      ordered_objects;
  std::map<ObjectId, std::set<EventPtr>> thread_locked_objects;
};
SYAN_REGISTER_CHECK(MutexLockOrderCheck);
