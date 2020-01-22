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
  void handle_event_before_checks(const EventPtr& event);
  void handle_event_after_checks(const EventPtr& event);

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
  std::string object_name(ObjectType object_type, ObjectId object_id) const;

  struct ThreadState {
    EventPtr create;
    EventPtr detach;
  };
  std::map<ObjectId, ThreadState> active_threads;
  std::map<std::pair<ObjectType, ObjectId>, EventPtr> active_objects;
  std::map<ObjectType, std::size_t> last_used_name;
  std::map<std::pair<ObjectType, ObjectId>, std::size_t> object_names;
};

} // namespace syan

#endif
