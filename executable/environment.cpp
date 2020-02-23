#include "environment.hpp"

#include <iomanip>
#include <iostream>

#include "check.hpp"

namespace syan {

Environment::Environment(std::optional<std::string> binary_file_path,
                         std::string dump_file_path)
    : binary_file_path(std::move(binary_file_path)),
      dump_file_path(std::move(dump_file_path)) {
  for (auto* registered_check = internal::RegisteredCheck::get_head();
       registered_check != nullptr;
       registered_check = registered_check->next_check) {
    enabled_checks.push_back(registered_check->check);
  }
}

void Environment::analyze() {
  EventFileReader dump_file_reader(dump_file_path);
  file_header.load(std::move(dump_file_reader.release_header()));
  tm* calendarTime = gmtime(&file_header.start_time.tv_sec);
  std::cout << "Sync analysis version " SYNC_ANALYSIS_VERSION "\n"
            << "\tExecutable: " << file_header.program_name << "\n"
            << "\tCommand line: " << file_header.program_command << "\n"
#ifdef SYNC_ANALYSIS_IS_MAC_OS_X
            << "\tExecutable load address: " << file_header.program_load_addr
            << " (0x" << std::hex << std::setfill('0') << std::setw(16)
            << file_header.program_load_addr << std::setfill(' ') << std::dec
            << ")\n"
#endif
            << "\tExecution start time: "
            << std::put_time(calendarTime, "%d/%m/%Y %H:%M:%S") << "."
            << std::setfill('0') << std::setw(6)
            << file_header.start_time.tv_nsec / 1000 << std::setw(0)
            << std::setfill(' ') << "\n\n";

  for (auto* check : enabled_checks) {
    check->on_start(*this);
  }

  while (!dump_file_reader.done()) {
    Event event = Event::make(dump_file_reader.read());
    if (!event) {
      // It's ok for the last event to be corrupt, maybe something was broken.
      if (!dump_file_reader.done()) {
        std::cout << "FATAL: Dump file " << dump_file_path << " is corrupt.\n";
      } else {
        return;
      }
    }
    active_objects_db.handle_event_before_checks(event);
    for (auto* check : enabled_checks) {
      check->on_event(*this, event);
    }
    active_objects_db.handle_event_after_checks(event);
  }

  for (auto* check : enabled_checks) {
    check->on_end(*this);
  }
}

Report Environment::create_report(Report::Level level, int code,
                                  std::string description) {
  return Report{this, level, code, std::move(description)};
}

const ActiveObjectsDb& Environment::db() const noexcept {
  return active_objects_db;
}

void Environment::send_report(Report::Level level, int /*code*/,
                              const std::string& report_message) const {
  switch (level) {
  case Report::info: std::cout << "INFO: " << report_message << "\n"; break;
  case Report::warning:
    std::cout << "WARNING: " << report_message << "\n";
    break;
  case Report::error: std::cout << "ERROR: " << report_message << "\n"; break;
  }
}

void Environment::symbolize_stacktrace(const Event& event, std::ostream& out) {
  if (!binary_file_path.has_value()) {
    for (const auto& pc : event.raw_backtrace()) {
      if (pc != 0) {
        out << "\t\t " << pc << " (0x" << std::hex << std::setfill('0')
            << std::setw(16) << pc << ")\n"
            << std::dec;
      }
    }
    return;
  }

  if (stacktrace_symbolizer == nullptr) {
    stacktrace_symbolizer =
        StacktraceSymbolizer::Create(*binary_file_path, file_header);
  }

  stacktrace_symbolizer->symbolize_stacktrace(event, out);
}

}  // namespace syan
