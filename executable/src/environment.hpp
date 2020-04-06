#ifndef SYNC_ANALYSIS_EXECUTABLE_SRC_ENVIRONMENT_H_
#define SYNC_ANALYSIS_EXECUTABLE_SRC_ENVIRONMENT_H_

#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include "dump_file_header_compat.hpp"
#include "extension.hpp"
#include "stacktrace_symbolizer.hpp"
#include "syan_extension_api/database.hpp"
#include "syan_extension_api/event.hpp"
#include "syan_extension_api/report.hpp"

namespace syan {

class Environment {
public:
  Environment(const std::optional<std::string>& binary_file_path,
              const DumpFileHeader& header, std::vector<Extension>&& extensions,
              std::ostream* report_stream);

  void start_up();

  void handle_event(Event event);

  int shut_down();

  void send_report(Report::Level level, const std::string& report_message);

  void symbolize_stacktrace(const Event& event, std::ostream& stream) const;

  const Database& get_database() const;

  Event current_event() const;

  struct timespec execution_start_time() const;

  std::string_view active_extension_name() const;

private:
  struct timespec const start_time;
  std::ostream* const report_stream;
  std::vector<Extension> const extensions;
  std::unique_ptr<StacktraceSymbolizer> const stacktrace_symbolizer;
  std::unique_ptr<Database> const database = std::make_unique<Database>();

  Event cur_event;
  const Extension* active_extension = nullptr;
  int exit_code = 0;
};

}  // namespace syan

#endif
