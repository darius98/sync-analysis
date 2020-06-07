#ifndef SYNC_ANALYSIS_EXECUTABLE_SRC_ENVIRONMENT_H_
#define SYNC_ANALYSIS_EXECUTABLE_SRC_ENVIRONMENT_H_

#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include "analyzer.hpp"
#include "dump_file_header_compat.hpp"
#include "stacktrace_symbolizer.hpp"
#include "syan_analyzer_api/database.hpp"
#include "syan_analyzer_api/event.hpp"
#include "syan_analyzer_api/report.hpp"

namespace syan {

class Environment {
public:
  Environment(const std::optional<std::string>& binary_file_path,
              const DumpFileHeader& header, std::vector<Analyzer>&& analyzers,
              std::ostream* report_stream);

  void start_up();

  void handle_event(Event event);

  int shut_down();

  void send_report(Report::Level level, const std::string& report_message);

  void symbolize_stacktrace(const Event& event, std::ostream& stream) const;

  const Database& get_database() const;

  Event current_event() const;

  struct timespec execution_start_time() const;

  std::string_view active_analyzer_name() const;

private:
  struct timespec const start_time;
  std::ostream* const report_stream;
  std::vector<Analyzer> const analyzers;
  std::unique_ptr<StacktraceSymbolizer> const stacktrace_symbolizer;
  std::unique_ptr<Database> const database = std::make_unique<Database>();

  Event cur_event;
  const Analyzer* active_analyzer = nullptr;
  int exit_code = 0;
};

}  // namespace syan

#endif
