#ifndef SYNC_ANALYSIS_EXE_ACTIVE_OBJECTS_DB_H_
#define SYNC_ANALYSIS_EXE_ACTIVE_OBJECTS_DB_H_

#include <map>
#include <string>
#include <string_view>

#include "event.hpp"

namespace syan {

class ActiveObjectsDb {
public:
  void insert(EventPtr event);

  std::string thread_name(ObjectId thread_id) const;

  std::string mutex_name(ObjectId mutex_id) const;

  std::string object_name(const Event& event) const;

  EventPtr thread_create(ObjectId thread_id) const noexcept;

  EventPtr thread_detach(ObjectId thread_id) const noexcept;

  EventPtr mutex_create(ObjectId mutex_id) const noexcept;

  EventPtr rec_mutex_create(ObjectId rec_mutex_id) const noexcept;

  EventPtr rwlock_create(ObjectId rwlock_id) const noexcept;

private:
  std::string object_name(std::string_view object_type, ObjectId object_id) const;

  struct ThreadState {
    EventPtr create;
    EventPtr detach;
  };
  std::map<ObjectId, ThreadState> active_threads;

  std::map<ObjectId, EventPtr> active_mutexes;
  std::map<ObjectId, EventPtr> active_rec_mutexes;
  std::map<ObjectId, EventPtr> active_rwlocks;
};

} // namespace syan

#endif
