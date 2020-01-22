#ifndef SYNC_ANALYSIS_EXE_REPORT_H_
#define SYNC_ANALYSIS_EXE_REPORT_H_

#include <set>
#include <string>
#include <vector>

#include "event.hpp"

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
                  std::string description);

  void add_section(std::string section_description, Event event);

  void send();

private:
  struct ReportSection {
    std::string description;
    Event event;

    ReportSection(std::string description, Event event);
  };

  const Environment* env;
  Level level;
  int code;
  std::string description;
  std::vector<ReportSection> sections;
  std::set<ObjectId> thread_notes;
  std::set<Event> notes;
};

} // namespace syan

#endif
