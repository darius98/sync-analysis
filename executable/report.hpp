#ifndef SYNC_ANALYSIS_EXE_REPORT_H_
#define SYNC_ANALYSIS_EXE_REPORT_H_

#include <string>
#include <vector>

#include "logger.hpp"
#include <lib/src/event.h>

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

  void send();

private:
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
};

} // namespace syan

#endif
