#ifndef SYNC_ANALYSIS_EXECUTABLE_SRC_STACKTRACE_SYMBOLIZER_H_
#define SYNC_ANALYSIS_EXECUTABLE_SRC_STACKTRACE_SYMBOLIZER_H_

#include <memory>
#include <string>

#include "dump_file_header_compat.hpp"
#include <syan_extension_api/event.hpp>

namespace syan {

// TODO: Implement this for Linux too.

class StacktraceSymbolizer {
public:
  static StacktraceSymbolizer* Create(const std::string& binary_file_path,
                                      const DumpFileHeader& dump_file_header);

  virtual ~StacktraceSymbolizer() = default;

  virtual void symbolize_stacktrace(RawBacktrace stack_trace,
                                    std::ostream& out) = 0;
};

}  // namespace syan

#endif
