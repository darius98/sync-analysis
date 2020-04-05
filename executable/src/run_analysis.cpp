#include "executable/include/syan_extension_api/syan_extension_api.hpp"

#include <iomanip>
#include <iostream>

#include "debug.hpp"
#include "event_file_reader.hpp"
#include "extension.hpp"
#include "stacktrace_symbolizer.hpp"

namespace {

using namespace syan;

class Environment {
public:
  Environment(const std::optional<std::string>& binary_file_path,
              const DumpFileHeader& header, std::vector<Extension>&& extensions,
              std::ostream* report_stream)
      : start_time(header.start_time),
        report_stream(report_stream),
        extensions(std::move(extensions)),
        stacktrace_symbolizer(
            StacktraceSymbolizer::create(binary_file_path, header)) {}

  void start_up() {
    for (const auto& extension : extensions) {
      active_extension = &extension;
      extension.start_up();
      active_extension = nullptr;
    }
  }

  void handle_event(Event event) {
    cur_event = std::move(event);
    database->handle_event_before_extensions(cur_event);
    for (const auto& extension : extensions) {
      active_extension = &extension;
      extension.on_event();
      active_extension = nullptr;
    }
    database->handle_event_after_extensions(cur_event);
    cur_event = nullptr;
  }

  void shut_down() {
    for (const auto& extension : extensions) {
      active_extension = &extension;
      extension.shut_down();
      active_extension = nullptr;
    }
  }

  void send_report(Report::Level level, const std::string& report_message) {
    *report_stream << report_message << "\n";
    if (level != Report::Level::info) {
      exit_code = 1;
    }
  }

  void symbolize_stacktrace(const Event& event, std::ostream& stream) const {
    stacktrace_symbolizer->symbolize_stacktrace(event.raw_backtrace(), stream);
  }

  const Database& get_database() const {
    return *database;
  }

  Event current_event() const {
    return cur_event;
  }

  struct timespec execution_start_time() const {
    return start_time;
  }

  std::string_view active_extension_name() const {
    return active_extension->get_name();
  }

  int get_exit_code() const {
    return exit_code;
  }

private:
  struct timespec const start_time;
  std::ostream* const report_stream;
  std::vector<Extension> const extensions;
  std::unique_ptr<StacktraceSymbolizer> const stacktrace_symbolizer;
  std::unique_ptr<Database> database = std::make_unique<Database>();

  Event cur_event;
  const Extension* active_extension = nullptr;
  int exit_code = 0;
};

Environment* environment = nullptr;

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

int run_analysis(std::optional<std::string> binary_file_path,
                 std::string dump_file_path, std::vector<Extension> extensions,
                 std::ostream* report_stream) {
  DOUT << "Reading dump file at " << dump_file_path;
  EventFileReader dump_file_reader(dump_file_path);
  DOUT << "Finished reading dump file";
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
            << std::setfill(' ') << "\n";
  DOUT << "Parsed dump file header";
  if (extensions.empty()) {
    std::cout << "\nNo extensions enabled. Nothing to do.\n";
    return 0;
  }

  environment = new Environment(binary_file_path, file_header,
                                std::move(extensions), report_stream);

  DOUT << "Created stacktrace symbolizer process";

  environment->start_up();

  while (!dump_file_reader.done()) {
    auto event = dump_file_reader.read();
    if (!event) {
      // It's ok for the last event to be corrupt, maybe something was broken.
      if (!dump_file_reader.done()) {
        std::cout << "FATAL: Dump file " << dump_file_path << " is corrupt.\n";
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
