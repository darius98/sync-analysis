#ifndef SYNC_ANALYSIS_EXE_ENVIRONMENT_H_
#define SYNC_ANALYSIS_EXE_ENVIRONMENT_H_

#include <ostream>
#include <string>

#include "active_objects_db.hpp"
#include "event_file_reader.hpp"
#include "logger.hpp"
#include "report.hpp"

namespace syan {

class Check;

class Environment {
public:
  Environment(std::string binary_file_path, std::string dump_file_path);

  void analyze();

  Report create_report(Report::Level level, int code,
                       std::string description) const noexcept;

  const ActiveObjectsDb& active_objects_db() const noexcept;

private:
  void send_report(Report::Level level, int code,
                   const std::string& report_message) const;

  void symbolize_backtrace_to_stream(const Event& event,
                                     std::ostream& stream) const;

  std::string binary_file_path;
  std::string dump_file_path;
  std::vector<Check*> enabled_checks;
  Logger logger;
  ActiveObjectsDb db;

  friend class Report;
};

} // namespace syan

#endif
