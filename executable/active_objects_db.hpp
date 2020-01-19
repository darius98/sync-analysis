#ifndef SYNC_ANALYSIS_EXE_ACTIVE_OBJECTS_DB_H_
#define SYNC_ANALYSIS_EXE_ACTIVE_OBJECTS_DB_H_

#include <map>
#include <string>
#include <string_view>

#include "event.hpp"
#include "utils.hpp"

namespace syan {

class ActiveObjectsDb {
public:
  void insert(const EventPtr& event);

  std::string thread_name(ObjectId thread_id) const;
  std::string thread_name(const Event& event) const;
  std::string thread_name(const EventPtr& event) const;

  EventPtr thread_create(ObjectId thread_id) const noexcept;
  EventPtr thread_create(const Event& event) const noexcept;
  EventPtr thread_create(const EventPtr& event) const noexcept;

  EventPtr thread_detach(ObjectId thread_id) const noexcept;
  EventPtr thread_detach(const Event& event) const noexcept;
  EventPtr thread_detach(const EventPtr& event) const noexcept;

  std::string object_name(const Event& event) const;
  std::string object_name(const EventPtr& event) const;

  EventPtr object_create(const Event& event) const noexcept;
  EventPtr object_create(const EventPtr& event) const noexcept;

private:
  std::string object_name(std::string_view object_type,
                          ObjectId object_id) const;

  struct ThreadState {
    EventPtr create;
    EventPtr detach;
  };
  std::map<ObjectId, ThreadState> active_threads;
  std::map<std::pair<ObjectType, ObjectId>, EventPtr> active_objects;
};

} // namespace syan

#endif
