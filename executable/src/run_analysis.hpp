#ifndef SYNC_ANALYSIS_EXECUTABLE_SRC_RUN_ANALYSIS_H_
#define SYNC_ANALYSIS_EXECUTABLE_SRC_RUN_ANALYSIS_H_

#include <optional>
#include <ostream>
#include <string>

#include "extension.hpp"
#include "syan_extension_api/event.hpp"
#include "syan_extension_api/report.hpp"

namespace syan {

struct timespec execution_start_time() noexcept;

std::string_view active_extension_name() noexcept;

void symbolize_stacktrace(const Event& event, std::ostream& stream);

void send_report(Report::Level level, const std::string& report_message);

int run_analysis(std::optional<std::string> binary_file_path,
                 std::string dump_file_path, std::vector<Extension> extensions,
                 std::ostream* report_stream,
                 std::string_view report_stream_name, bool print_header);

}  // namespace syan

#endif
