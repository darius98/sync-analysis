#ifndef SYNC_ANALYSIS_EXE_ACTIVE_OBJECTS_DB_H_
#define SYNC_ANALYSIS_EXE_ACTIVE_OBJECTS_DB_H_

#include <map>
#include <optional>
#include <string>

#include "event.hpp"

namespace syan {

class ActiveObjectsDb {
public:
  void insert(EventPtr event);

  std::string get_thread_name(ThreadId thread_id) const;

  EventPtr get_thread_create_event(ThreadId thread_id) const noexcept;

  EventPtr get_thread_detach_event(ThreadId thread_id) const noexcept;

  EventPtr get_mutex_create_event(ObjectId mutex_id) const noexcept;

  EventPtr get_rec_mutex_create_event(ObjectId rec_mutex_id) const noexcept;

  EventPtr get_rwlock_create_event(ObjectId rwlock_id) const noexcept;

private:
  std::map<ThreadId, EventPtr> active_threads_by_id;
  std::map<ObjectId, EventPtr> active_threads;
  std::map<ObjectId, EventPtr> detached_threads;

  std::map<ObjectId, EventPtr> active_mutexes;
  std::map<ObjectId, EventPtr> active_rec_mutexes;
  std::map<ObjectId, EventPtr> active_rwlocks;
};

} // namespace syan

#endif
