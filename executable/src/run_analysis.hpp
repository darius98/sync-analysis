#ifndef SYNC_ANALYSIS_EXECUTABLE_SRC_RUN_ANALYSIS_H_
#define SYNC_ANALYSIS_EXECUTABLE_SRC_RUN_ANALYSIS_H_

#include <optional>
#include <ostream>
#include <string>

#include "analyzer.hpp"
#include "options.hpp"
#include "syan_analyzer_api/event.hpp"
#include "syan_analyzer_api/report.hpp"

namespace syan {

struct timespec execution_start_time();

std::string_view active_analyzer_name();

void symbolize_stacktrace(const Event& event, std::ostream& stream);

void send_report(Report::Level level, const std::string& report_message);

int run_analysis(const Options& options, std::vector<Analyzer> analyzers);

}  // namespace syan

#endif
