#ifndef SYNC_ANALYSIS_EXE_ENVIRONMENT_H_
#define SYNC_ANALYSIS_EXE_ENVIRONMENT_H_

#include <ostream>
#include <string>

#include "event_file_reader.hpp"
#include "executable/include/check_api/report.hpp"
#include "include/check_api/database.hpp"
#include "stacktrace_symbolizer.hpp"

namespace syan {

class Check;

class Environment {
public:
  Environment(std::optional<std::string> binary_file_path,
              std::string dump_file_path);

  void analyze();

  Report create_report(Report::Level level, int code, std::string description);

  const Database& db() const noexcept;

private:
  void send_report(Report::Level level, int code,
                   const std::string& report_message) const;

  void symbolize_stacktrace(const Event& event, std::ostream& stream);

  std::optional<std::string> binary_file_path;
  std::string dump_file_path;
  std::vector<Check*> enabled_checks;
  DumpFileHeader file_header;
  Database active_objects_db;
  std::unique_ptr<StacktraceSymbolizer> stacktrace_symbolizer;

  friend class Report;
};

void run_analysis(std::optional<std::string> binary_file_path,
                  std::string dump_file_path);

}  // namespace syan

#endif
