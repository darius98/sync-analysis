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

  void add_section(std::string section_description, EventPtr event);

  void add_mutex_note(ObjectId mutex_id);

  void send();

private:
  void add_unique_object_note(const char* object_type, EventPtr event);

  struct ReportSection {
    std::string description;
    EventPtr event;

    ReportSection(std::string description, EventPtr event);
  };

  const Environment* env;
  Level level;
  int code;
  std::string description;
  std::vector<ReportSection> sections;
  std::set<ThreadId> thread_notes;
  std::set<std::tuple<const char*, EventPtr>> object_notes;
};

} // namespace syan

#endif
