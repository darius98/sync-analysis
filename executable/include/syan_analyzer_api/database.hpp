#ifndef SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_ANALYZER_API_DATABASE_H_
#define SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_ANALYZER_API_DATABASE_H_

#include <map>
#include <string>
#include <string_view>

#include <syan_analyzer_api/event.hpp>

namespace syan {

class Database {
public:
  void handle_event_before_analyzers(const Event& event);
  void handle_event_after_analyzers(const Event& event);

  std::string thread_name(ObjectId thread_id) const;
  std::string thread_name(const Event& event) const;

  Event thread_create(ObjectId thread_id) const noexcept;
  Event thread_create(const Event& event) const noexcept;

  std::string object_name(const Event& event) const;
  std::string object_name(ObjectType object_type, ObjectId object_id) const;

  Event object_create(const Event& event) const noexcept;
  Event object_create(ObjectType object_type, ObjectId object_id) const
      noexcept;

private:
  std::map<std::pair<ObjectType, ObjectId>, Event> active_objects;
  std::map<ObjectType, std::size_t> last_used_name;
  std::map<std::pair<ObjectType, ObjectId>, std::size_t> object_names;
};

}  // namespace syan

#endif
