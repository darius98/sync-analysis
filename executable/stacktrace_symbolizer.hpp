#ifndef SYNC_ANALYSIS_EXE_STACKTRACE_SYMBOLIZER_H_
#define SYNC_ANALYSIS_EXE_STACKTRACE_SYMBOLIZER_H_

#include <memory>
#include <string>

#include "check_api/event.hpp"
#include "dump_file_header_compat.hpp"

namespace syan {

// TODO: Implement this for Linux too.

class StacktraceSymbolizer {
public:
  static StacktraceSymbolizer* Create(const std::string& binary_file_path,
                                      const DumpFileHeader& dump_file_header);

  virtual ~StacktraceSymbolizer() = default;

  virtual void symbolize_stacktrace(const Event& event, std::ostream& out) = 0;
};

}  // namespace syan

#endif
