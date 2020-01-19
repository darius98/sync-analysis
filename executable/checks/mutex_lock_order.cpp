#include <map>
#include <set>

#include <executable/check.hpp>

using namespace syan;

class MutexLockOrderCheck : public Check {
public:
  void on_event(const Environment& env, const Event& event) final {
    switch (event.event_type) {
    case SA_EV_MUTEX_BEFORE_LOCK: {
      auto& locked_on_thread = thread_locked_objects[event.thread_id];
      for (const auto& prev_lock_event : locked_on_thread) {
        auto it = ordered_objects.find({event.addr, prev_lock_event.addr});
        if (it != ordered_objects.end()) {
          const auto& [mtx1_lock, mtx2_lock] = it->second;

          auto report = env.create_report(
              Report::error, 1, "Mutex lock order inconsistency found");

          report.add_section("Thread " +
                                 env.db().get_thread_name(mtx1_lock.thread_id) +
                                 " locked mutex 1 first here:",
                             mtx1_lock);
          report.add_section("Thread " +
                                 env.db().get_thread_name(mtx2_lock.thread_id) +
                                 " locked mutex 2 second here:",
                             mtx2_lock);

          report.add_section(
              "Thread " + env.db().get_thread_name(prev_lock_event.thread_id) +
                  " locked mutex 2 first here:",
              prev_lock_event);

          report.add_section("Thread " +
                                 env.db().get_thread_name(event.thread_id) +
                                 " attempted to lock mutex 1 second here:",
                             event);

          report.add_mutex_note("1", mtx1_lock.addr);

          report.add_mutex_note("2", mtx2_lock.addr);

          report.send();
        }
      }
      break;
    }
    case SA_EV_MUTEX_AFTER_LOCK: {
      thread_locked_objects[event.thread_id].insert(event);
      auto& locked_on_thread = thread_locked_objects[event.thread_id];
      for (const auto& prev_lock_event : locked_on_thread) {
        ordered_objects.try_emplace(std::pair{prev_lock_event.addr, event.addr},
                                    std::pair{prev_lock_event, event});
      }
      break;
    }
    case SA_EV_MUTEX_ON_UNLOCK: {
      thread_locked_objects[event.thread_id].erase(event);
      break;
    }
    default:
      break;
    }
  }

private:
  struct EventCompareByAddr {
    bool operator()(const Event& a, const Event& b) const noexcept {
      return a.addr < b.addr;
    }
  };

  std::map<std::pair<ObjectId, ObjectId>, std::pair<Event, Event>>
      ordered_objects;
  std::map<ThreadId, std::set<Event, EventCompareByAddr>> thread_locked_objects;
};
SYAN_REGISTER_CHECK(MutexLockOrderCheck);
