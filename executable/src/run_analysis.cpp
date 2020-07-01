#include "run_analysis.hpp"
#include "syan_analyzer_api/syan_analyzer_api.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>

#include "debug.hpp"
#include "environment.hpp"
#include "event_file_reader.hpp"

namespace syan {

namespace {

Environment* environment = nullptr;

}  // namespace

Event current_event() {
  return environment->current_event();
}

const Database& database() {
  return environment->get_database();
}

Report create_report() {
  return Report{};
}

struct timespec execution_start_time() {
  return environment->execution_start_time();
}

std::string_view active_analyzer_name() {
  return environment->active_analyzer_name();
}

void symbolize_stacktrace(const Event& event, std::ostream& stream) {
  environment->symbolize_stacktrace(event, stream);
}

void send_report(Report::Level level, const std::string& report_message) {
  environment->send_report(level, report_message);
}

int run_analysis(const Options& options, std::vector<Analyzer> analyzers) {
  std::ostream* report_stream = &std::cout;
  std::unique_ptr<std::ostream> own_report_stream;
  if (options.report_file_path.has_value()) {
    own_report_stream.reset(new std::ofstream(*options.report_file_path));
    if (!*own_report_stream) {
      FATAL_OUT << "Cannot open path to report file for test writing: "
                << *options.report_file_path;
    }
    report_stream = own_report_stream.get();
  }

  DOUT << "Opening dump file at " << options.dump_file_path;
  EventFileReader dump_file_reader(options.dump_file_path);
  DumpFileHeader file_header(std::move(dump_file_reader.release_header()));
  DOUT << "Finished reading header of dump file";

  if (options.print_header || debugging::is_debug_enabled()) {
    std::tm* calendarTime = std::gmtime(&file_header.start_time.tv_sec);
    std::cout << "Sync analysis version " SYNC_ANALYSIS_VERSION "\n"
              << "\tExecutable: " << file_header.program_name << "\n"
              << "\tCommand line: " << file_header.program_command << "\n"
              << "\tExecutable load address: " << file_header.program_load_addr
              << " (0x" << std::hex << std::setfill('0') << std::setw(16)
              << file_header.program_load_addr << std::setfill(' ') << std::dec
              << ")\n"
              << "\tExecution start time: "
              << std::put_time(calendarTime, "%d/%m/%Y %H:%M:%S") << "."
              << std::setfill('0') << std::setw(6)
              << file_header.start_time.tv_nsec / 1000 << std::setw(0)
              << std::setfill(' ') << "\n"
              << "\tDump file: " << options.dump_file_path << "\n"
              << "\tReports are written to: "
              << options.report_file_path.value_or("STDOUT") << "\n";
  }
  if (analyzers.empty()) {
    FATAL_OUT << "No analyzers enabled. Nothing to do.";
  }

  environment = new Environment(options.binary_file_path, file_header,
                                std::move(analyzers), report_stream);

  environment->start_up();

  while (!dump_file_reader.done()) {
    auto event = dump_file_reader.read();
    if (!event) {
      // It's ok for the last event to be corrupt, maybe something was broken.
      if (!dump_file_reader.done()) {
        FATAL_OUT << "Dump file " << options.dump_file_path << " is corrupt.";
      } else {
        break;
      }
    }

    environment->handle_event(std::move(event));
  }

  int exit_code = environment->shut_down();

  delete environment;
  environment = nullptr;

  return exit_code;
}

}  // namespace syan
