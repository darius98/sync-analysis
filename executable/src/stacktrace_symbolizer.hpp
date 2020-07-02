#ifndef SYNC_ANALYSIS_EXECUTABLE_SRC_STACKTRACE_SYMBOLIZER_H_
#define SYNC_ANALYSIS_EXECUTABLE_SRC_STACKTRACE_SYMBOLIZER_H_

#include <memory>
#include <optional>
#include <ostream>
#include <string>

#include "dump_file_header_compat.hpp"
#include "syan_analyzer_api/event.hpp"

namespace syan {

class StacktraceSymbolizer {
public:
  static std::unique_ptr<StacktraceSymbolizer>
  create(const std::optional<std::string>& binary_file_path,
         const DumpFileHeader& dump_file_header);

  virtual ~StacktraceSymbolizer() = default;

  virtual void symbolize_stacktrace(RawBacktrace stack_trace,
                                    std::ostream& out) = 0;

private:
  static std::unique_ptr<StacktraceSymbolizer>
  do_create(const std::string& binary_file_path,
            const DumpFileHeader& dump_file_header);
};

}  // namespace syan

#endif
