#ifndef SYNC_ANALYSIS_EXE_ENVIRONMENT_H_
#define SYNC_ANALYSIS_EXE_ENVIRONMENT_H_

#include <ostream>
#include <string>

#include "event_file_reader.hpp"
#include "logger.hpp"
#include "report.hpp"

namespace syan {

class Environment {
public:
  Environment(std::string binary_file_path, const std::string& dump_file_path);

  void analyze();

  Report create_report(Report::Level level, int code,
                       std::string description) const;

private:
  void send_report(Report::Level level,
                   int code,
                   const std::string& report_message) const;

  void symbolize_backtrace_to_stream(const Event& event,
                                     std::ostream& stream) const;

  std::string binary_file_path;
  std::string dump_file_path;
  EventFileReader dump_file_reader;
  Logger logger;

  friend class Report;
};

} // namespace syan

#endif
