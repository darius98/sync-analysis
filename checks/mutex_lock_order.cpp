#include <map>
#include <set>

#include <check_api/check_api.hpp>

using namespace syan;

class MutexLockOrderCheck: public Check {
public:
  void on_event(Event event) final {
    switch (event.type()) {
    case EventType::mutex_before_lock: {
      auto& locked_on_thread = thread_locked_objects[event.thread()];
      for (const auto& prev_lock_event : locked_on_thread) {
        auto it =
            ordered_objects.find({event.object(), prev_lock_event.object()});
        if (it != ordered_objects.end()) {
          const auto& [mtx1_lock, mtx2_lock] = it->second;

          Report report(Report::error, 1,
                        "Mutex lock order inconsistency found");

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

          report.add_section(
              database().thread_name(event) + " attempted to lock " +
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

private:
  std::map<std::pair<ObjectId, ObjectId>, std::pair<Event, Event>>
      ordered_objects;
  std::map<ObjectId, std::set<Event>> thread_locked_objects;
};
SYAN_REGISTER_CHECK(MutexLockOrderCheck);
