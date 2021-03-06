#include <map>
#include <set>

#include <syan_analyzer_api/syan_analyzer_api.hpp>

#include "utils.hpp"

using namespace syan;

struct MutexLockOrderAnalyzer {
  static constexpr const char* name = "mutex-lock-order";

  std::map<std::pair<ObjectId, ObjectId>, std::pair<Event, Event>> edges;
  std::map<ObjectId, std::set<Event>> thread_owned_mutexes;

  void operator()(Event event) {
    switch (event.type()) {
    case SA_EV_MUTEX_BEFORE_LOCK: {
      auto& owned = thread_owned_mutexes[event.thread()];
      for (const auto& owned_obj : owned) {
        auto it = edges.find({event.object(), owned_obj.object()});
        if (it != edges.end()) {
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
          report.add_section(database().thread_name(owned_obj) + " locked " +
                                 database().object_name(owned_obj) +
                                 " first here:",
                             owned_obj);
          report.add_section(
              database().thread_name(event) + " attempted to lock " +
                  database().object_name(event) + " second here:",
              event);
        }
      }
      break;
    }
    case SA_EV_MUTEX_AFTER_LOCK: {
      thread_owned_mutexes[event.thread()].insert(event);
      auto& owned = thread_owned_mutexes[event.thread()];
      for (const auto& owned_obj : owned) {
        edges.try_emplace(std::pair{owned_obj.object(), event.object()},
                          std::pair{owned_obj, event});
      }
      break;
    }
    case SA_EV_MUTEX_ON_UNLOCK: {
      thread_owned_mutexes[event.thread()].erase(event);
      break;
    }
    default: break;
    }
  }
};

SYAN_ANALYZER_CLASS(MutexLockOrderAnalyzer);
