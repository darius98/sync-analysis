#ifndef SYNC_ANALYSIS_EXE_ENVIRONMENT_H_
#define SYNC_ANALYSIS_EXE_ENVIRONMENT_H_

#include <string>

#include "logger.hpp"
#include "event_file_reader.hpp"

namespace syan {

class Environment {
public:
  Environment(std::string binary_file_path, const std::string& dump_file_path);

  void analyze();

private:
  [[noreturn]] void report_corrupt_file(std::int_fast32_t signature);

  std::string binary_file_path;
  std::string dump_file_path;
  EventFileReader dump_file_reader;
  Logger logger;
};

} // namespace syan

#endif
