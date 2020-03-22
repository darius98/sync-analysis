#ifndef SYNC_ANALYSIS_EXE_RUN_ANALYSIS_H_
#define SYNC_ANALYSIS_EXE_RUN_ANALYSIS_H_

#include <iostream>
#include <optional>
#include <string>

#include "check_api/check_api.hpp"

namespace syan {

struct timespec execution_start_time() noexcept;

void symbolize_stacktrace(const Event& event, std::ostream& stream);

void send_report(Report::Level level, int /*code*/,
                 const std::string& report_message);

void run_analysis(std::optional<std::string> binary_file_path,
                  std::string dump_file_path,
                  const std::vector<Check*>& checks);

}  // namespace syan

#endif
