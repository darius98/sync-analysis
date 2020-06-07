#include "environment.hpp"

namespace syan {

Environment::Environment(const std::optional<std::string>& binary_file_path,
                         const DumpFileHeader& header,
                         std::vector<Analyzer>&& analyzers,
                         std::ostream* report_stream)
    : start_time(header.start_time),
      report_stream(report_stream),
      analyzers(std::move(analyzers)),
      stacktrace_symbolizer(
          StacktraceSymbolizer::create(binary_file_path, header)) {}

void Environment::start_up() {
  for (const auto& analyzer : analyzers) {
    active_analyzer = &analyzer;
    analyzer.start_up();
    active_analyzer = nullptr;
  }
}

void Environment::handle_event(Event event) {
  cur_event = std::move(event);
  database->handle_event_before_analyzers(cur_event);
  for (const auto& analyzer : analyzers) {
    active_analyzer = &analyzer;
    analyzer.on_event();
    active_analyzer = nullptr;
  }
  database->handle_event_after_analyzers(cur_event);
  cur_event = nullptr;
}

int Environment::shut_down() {
  for (const auto& analyzer : analyzers) {
    active_analyzer = &analyzer;
    analyzer.shut_down();
    active_analyzer = nullptr;
  }
  return exit_code;
}

void Environment::send_report(Report::Level level,
                              const std::string& report_message) {
  *report_stream << report_message << "\n";
  if (level != Report::Level::info) {
    exit_code = 1;
  }
}

void Environment::symbolize_stacktrace(const Event& event,
                                       std::ostream& stream) const {
  stacktrace_symbolizer->symbolize_stacktrace(event.raw_backtrace(), stream);
}

const Database& Environment::get_database() const {
  return *database;
}

Event Environment::current_event() const {
  return cur_event;
}

struct timespec Environment::execution_start_time() const {
  return start_time;
}

std::string_view Environment::active_analyzer_name() const {
  return active_analyzer->get_name();
}

}  // namespace syan
