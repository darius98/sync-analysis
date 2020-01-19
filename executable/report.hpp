#ifndef SYNC_ANALYSIS_EXE_REPORT_H_
#define SYNC_ANALYSIS_EXE_REPORT_H_

#include <set>
#include <string>
#include <vector>

#include "event.hpp"
#include "logger.hpp"

namespace syan {

class Environment;

class Report {
public:
  enum Level {
    info,
    warning,
    error,
  };

  explicit Report(const Environment* env, Level level, int code,
                  std::string description) noexcept;

  void add_section(std::string section_description, const Event& event);

  void add_mutex_note(const std::string& mutex_name, ObjectId mutex_id);

  void send();

private:
  void add_unique_object_note(const char* object_type,
                              const std::string& object_name,
                              const Event& event);

  struct ReportSection {
    std::string description;
    Event event;

    ReportSection(std::string description, const Event& event);
  };

  const Environment* env;
  Level level;
  int code;
  std::string description;
  std::vector<ReportSection> sections;
  std::set<ThreadId> thread_notes;

  struct ObjectNoteCompare {
    bool operator()(const std::tuple<const char*, std::string, Event>& t1,
                    const std::tuple<const char*, std::string, Event>& t2) const
        noexcept;
  };
  std::set<std::tuple<const char*, std::string, Event>, ObjectNoteCompare>
      object_notes;
};

} // namespace syan

#endif
