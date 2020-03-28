#ifndef SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_EXTENSION_API_REPORT_H_
#define SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_EXTENSION_API_REPORT_H_

#include <set>
#include <string>
#include <vector>

#include <syan_extension_api/event.hpp>

namespace syan {

class Report {
public:
  enum Level {
    info,
    warning,
    error,
  };

  Report(Level level, std::string description);

  ~Report();

  Report& add_section(std::string section_description, Event event);

private:
  struct ReportSection {
    std::string description;
    Event event;

    ReportSection(std::string description, Event event)
        : description(std::move(description)), event(std::move(event)) {}
  };

  Level level;
  std::string description;
  std::vector<ReportSection> sections;
  std::set<ObjectId> thread_notes;
  std::set<Event> notes;
};

}  // namespace syan

#endif
