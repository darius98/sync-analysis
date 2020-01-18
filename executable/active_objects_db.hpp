#ifndef SYNC_ANALYSIS_EXE_ACTIVE_OBJECTS_DB_H_
#define SYNC_ANALYSIS_EXE_ACTIVE_OBJECTS_DB_H_

#include <map>
#include <optional>

#include <lib/src/event.h>

namespace syan {

class ActiveObjectsDb {
public:
  using ObjectId = decltype(Event::addr);
  using ThreadId = decltype(Event::thread_id);

  void insert(const Event& event);

  std::optional<Event>
  get_thread_create_event_by_thread_id(ThreadId thread_id) const noexcept;

  std::optional<Event>
  get_thread_detach_event_by_thread_id(ThreadId thread_id) const noexcept;

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
