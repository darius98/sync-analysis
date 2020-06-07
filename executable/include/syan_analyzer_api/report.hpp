#ifndef SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_ANALYZER_API_REPORT_H_
#define SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_ANALYZER_API_REPORT_H_

#include <set>
#include <string>
#include <vector>

#include <syan_analyzer_api/event.hpp>

namespace syan {

class Report {
public:
  enum Level {
    info,
    warning,
    error,
  };

  ~Report();

  Report& set_level(Level new_level) noexcept;
  Report& set_description(std::string new_description) noexcept;
  Report& add_section(std::string section_description, Event event);

private:
  Report() = default;

  struct ReportSection {
    std::string description;
    Event event;

    ReportSection(std::string description, Event event)
        : description(std::move(description)), event(std::move(event)) {}
  };

  Level level{Level::info};
  std::string description;
  std::vector<ReportSection> sections;
  std::set<ObjectId> thread_notes;
  std::set<Event> notes;

  friend Report create_report();
};

}  // namespace syan

#endif
