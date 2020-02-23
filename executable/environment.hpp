#ifndef SYNC_ANALYSIS_EXE_ENVIRONMENT_H_
#define SYNC_ANALYSIS_EXE_ENVIRONMENT_H_

#include <ostream>
#include <string>

#include "active_objects_db.hpp"
#include "event_file_reader.hpp"
#include "report.hpp"
#include "stacktrace_symbolizer.hpp"

namespace syan {

class Check;

class Environment {
public:
  Environment(std::optional<std::string> binary_file_path,
              std::string dump_file_path);

  void analyze();

  Report create_report(Report::Level level, int code, std::string description);

  const ActiveObjectsDb& db() const noexcept;

private:
  void send_report(Report::Level level, int code,
                   const std::string& report_message) const;

  void symbolize_stacktrace(const Event& event, std::ostream& stream);

  std::optional<std::string> binary_file_path;
  std::string dump_file_path;
  std::vector<Check*> enabled_checks;
  DumpFileHeader file_header;
  ActiveObjectsDb active_objects_db;
  std::unique_ptr<StacktraceSymbolizer> stacktrace_symbolizer;

  friend class Report;
};

}  // namespace syan

#endif
