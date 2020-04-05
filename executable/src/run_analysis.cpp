#include "run_analysis.hpp"
#include "syan_extension_api/syan_extension_api.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>

#include "debug.hpp"
#include "environment.hpp"
#include "event_file_reader.hpp"

namespace {

syan::Environment* environment = nullptr;

}  // namespace

namespace syan {

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

std::string_view active_extension_name() {
  return environment->active_extension_name();
}

void symbolize_stacktrace(const Event& event, std::ostream& stream) {
  environment->symbolize_stacktrace(event, stream);
}

void send_report(Report::Level level, const std::string& report_message) {
  environment->send_report(level, report_message);
}

int run_analysis(const Options& options, std::vector<Extension> extensions) {
  std::ostream* report_stream = &std::cout;
  std::unique_ptr<std::ostream> own_report_stream;
  if (options.report_file_path.has_value()) {
    own_report_stream.reset(
        new std::ofstream(options.report_file_path.value().c_str()));
    if (!*own_report_stream) {
      std::cout << "Cannot open path to report_file for test writing: "
                << options.report_file_path.value() << "\n";
      std::abort();
    }
    report_stream = own_report_stream.get();
  }

  DOUT << "Reading dump file at " << options.dump_file_path;
  EventFileReader dump_file_reader(options.dump_file_path);
  DOUT << "Finished reading dump file";
  DumpFileHeader file_header(std::move(dump_file_reader.release_header()));

  if (options.print_header) {
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
              << std::setfill(' ') << "\n"
              << "\tDump file: " << options.dump_file_path << "\n"
              << "\tReports are written to: "
              << options.report_file_path.value_or("STDOUT") << "\n";
  }
  DOUT << "Parsed dump file header";
  if (extensions.empty()) {
    std::cout << "\nNo extensions enabled. Nothing to do.\n";
    return 0;
  }

  environment = new Environment(options.binary_file_path, file_header,
                                std::move(extensions), report_stream);

  DOUT << "Created stacktrace symbolizer process";

  environment->start_up();

  while (!dump_file_reader.done()) {
    auto event = dump_file_reader.read();
    if (!event) {
      // It's ok for the last event to be corrupt, maybe something was broken.
      if (!dump_file_reader.done()) {
        std::cout << "FATAL: Dump file " << options.dump_file_path
                  << " is corrupt.\n";
      } else {
        break;
      }
    }

    environment->handle_event(std::move(event));
  }

  environment->shut_down();

  int exit_code = environment->get_exit_code();

  delete environment;
  environment = nullptr;

  return exit_code;
}

}  // namespace syan
