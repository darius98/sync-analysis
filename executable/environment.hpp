#ifndef SYNC_ANALYSIS_EXE_ENVIRONMENT_H_
#define SYNC_ANALYSIS_EXE_ENVIRONMENT_H_

#include <ostream>
#include <string>

#include "active_objects_db.hpp"
#include "event_file_reader.hpp"
#include "report.hpp"

namespace syan {

class Check;

class Environment {
public:
  Environment(std::optional<std::string> binary_file_path,
              std::string dump_file_path);

  void analyze();

  Report create_report(Report::Level level, int code,
                       std::string description) const;

  const ActiveObjectsDb& db() const noexcept;

private:
  void send_report(Report::Level level, int code,
                   const std::string& report_message) const;

  void symbolize_backtrace_to_stream(const Event& event,
                                     std::ostream& stream) const;

  std::optional<std::string> binary_file_path;
  std::string dump_file_path;
  std::vector<Check*> enabled_checks;
  DumpFileHeader file_header;
  ActiveObjectsDb active_objects_db;

  friend class Report;
};

}  // namespace syan

#endif
