#include "executable/include/syan_extension_api/syan_extension_api.hpp"

#include <iomanip>
#include <iostream>

#include "event_file_reader.hpp"
#include "extension.hpp"
#include "stacktrace_symbolizer.hpp"

namespace {

struct timespec start_time;
syan::Event cur_event;
syan::Database* active_objects_db = nullptr;
syan::StacktraceSymbolizer* stacktrace_symbolizer = nullptr;

}  // namespace

namespace syan {

const Database& database() noexcept {
  return *active_objects_db;
}

struct timespec execution_start_time() noexcept {
  return start_time;
}

Event current_event() noexcept {
  return cur_event;
}

void symbolize_stacktrace(const Event& event, std::ostream& stream) {
  if (stacktrace_symbolizer == nullptr) {
    stream << "\t\tNote: stacktrace not available without access to the "
              "original binary.\n";
    return;
  }
  stacktrace_symbolizer->symbolize_stacktrace(event.raw_backtrace(), stream);
}

void send_report(Report::Level level, int /*code*/,
                 const std::string& report_message) {
  switch (level) {
  case Report::info: std::cout << "INFO: " << report_message << "\n"; break;
  case Report::warning:
    std::cout << "WARNING: " << report_message << "\n";
    break;
  case Report::error: std::cout << "ERROR: " << report_message << "\n"; break;
  }
}

void run_analysis(std::optional<std::string> binary_file_path,
                  std::string dump_file_path,
                  const std::vector<Extension>& extensions) {
  EventFileReader dump_file_reader(dump_file_path);
  DumpFileHeader file_header(std::move(dump_file_reader.release_header()));
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

  start_time = file_header.start_time;

  active_objects_db = new Database();

  if (binary_file_path.has_value()) {
    stacktrace_symbolizer =
        StacktraceSymbolizer::Create(*binary_file_path, file_header);
  } else {
    stacktrace_symbolizer = nullptr;
  }

  for (const auto& extension : extensions) {
    extension.start_up();
  }

  while (!dump_file_reader.done()) {
    auto event = dump_file_reader.read();
    if (!event) {
      // It's ok for the last event to be corrupt, maybe something was broken.
      if (!dump_file_reader.done()) {
        std::cout << "FATAL: Dump file " << dump_file_path << " is corrupt.\n";
      } else {
        return;
      }
    }
    cur_event = event;

    active_objects_db->handle_event_before_extensions(event);
    for (const auto& extension : extensions) {
      extension.on_event();
    }
    active_objects_db->handle_event_after_extensions(event);
  }

  for (const auto& extension : extensions) {
    extension.shut_down();
  }

  delete active_objects_db;
  active_objects_db = nullptr;

  delete stacktrace_symbolizer;
  stacktrace_symbolizer = nullptr;
}

}  // namespace syan
