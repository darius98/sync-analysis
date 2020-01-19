#ifndef SYNC_ANALYSIS_EXE_ACTIVE_OBJECTS_DB_H_
#define SYNC_ANALYSIS_EXE_ACTIVE_OBJECTS_DB_H_

#include <map>
#include <optional>
#include <string>

#include "event.hpp"

namespace syan {

class ActiveObjectsDb {
public:
  void insert(const Event& event);

  std::string get_thread_name(ThreadId thread_id) const;

  std::optional<Event> get_thread_create_event(ThreadId thread_id) const
      noexcept;

  std::optional<Event> get_thread_detach_event(ThreadId thread_id) const
      noexcept;

  std::optional<Event> get_mutex_create_event(ObjectId mutex_id) const noexcept;

  std::optional<Event> get_rec_mutex_create_event(ObjectId rec_mutex_id) const
      noexcept;

  std::optional<Event> get_rwlock_create_event(ObjectId rwlock_id) const
      noexcept;

private:
  std::map<ThreadId, Event> active_threads_by_id;
  std::map<ObjectId, Event> active_threads;
  std::map<ObjectId, Event> detached_threads;

  std::map<ObjectId, Event> active_mutexes;
  std::map<ObjectId, Event> active_rec_mutexes;
  std::map<ObjectId, Event> active_rwlocks;
};

} // namespace syan

#endif
