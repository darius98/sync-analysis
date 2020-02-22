#ifndef SYNC_ANALYSIS_EXE_ACTIVE_OBJECTS_DB_H_
#define SYNC_ANALYSIS_EXE_ACTIVE_OBJECTS_DB_H_

#include <map>
#include <string>
#include <string_view>

#include "event.hpp"

namespace syan {

class ActiveObjectsDb {
public:
  void handle_event_before_checks(const Event& event);
  void handle_event_after_checks(const Event& event);

  std::string thread_name(ObjectId thread_id) const;
  std::string thread_name(const Event& event) const;

  Event thread_create(ObjectId thread_id) const noexcept;
  Event thread_create(const Event& event) const noexcept;

  std::string object_name(const Event& event) const;

  Event object_create(const Event& event) const noexcept;

private:
  std::string object_name(ObjectType object_type, ObjectId object_id) const;

  std::map<std::pair<ObjectType, ObjectId>, Event> active_objects;
  std::map<ObjectType, std::size_t> last_used_name;
  std::map<std::pair<ObjectType, ObjectId>, std::size_t> object_names;
};

}  // namespace syan

#endif
