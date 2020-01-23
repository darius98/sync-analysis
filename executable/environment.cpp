#include "environment.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

#include "check.hpp"

namespace syan {

Environment::Environment(std::string binary_file_path,
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
  file_header = dump_file_reader.get_header();
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
                                  std::string description) const {
  return Report{this, level, code, std::move(description)};
}

const ActiveObjectsDb& Environment::db() const noexcept {
  return active_objects_db;
}

void Environment::send_report(Report::Level level, int code,
                              const std::string& report_message) const {
  switch (level) {
  case Report::info:
    std::cout << "INFO: " << report_message << "\n";
    break;
  case Report::warning:
    std::cout << "WARNING: " << report_message << "\n";
    break;
  case Report::error:
    std::cout << "ERROR: " << report_message << "\n";
    break;
  }
}

void Environment::symbolize_backtrace_to_stream(const Event& event,
                                                std::ostream& stream) const {
  // TODO: Implement this for Linux too.

#ifdef SYNC_ANALYSIS_IS_MAC_OS_X
  std::stringstream atos_command_builder;
  // TODO: Don't hardcode architecture here!
  atos_command_builder << "atos -o " << binary_file_path
                       << " -arch x86_64 -l 0x" << std::hex << std::setfill('0')
                       << std::setw(16) << file_header.program_load_addr;
  for (const auto& pc : event.raw_backtrace()) {
    if (pc != 0) {
      atos_command_builder << " 0x" << std::hex << std::setfill('0')
                           << std::setw(16) << pc;
    }
  }
  stream << "\t\t";
  auto atos_command = atos_command_builder.str();
  std::FILE* atos_process = popen(atos_command.c_str(), "r");
  char ch;
  while ((ch = std::fgetc(atos_process)) != EOF) {
    stream << ch;
    if (ch == '\n') {
      stream << "\t\t";
    }
  }
  pclose(atos_process);
#endif
}

} // namespace syan
