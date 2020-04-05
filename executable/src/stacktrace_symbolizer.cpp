#include "stacktrace_symbolizer.hpp"

namespace syan {

class NullStacktraceSymbolizer: public StacktraceSymbolizer {
public:
  void symbolize_stacktrace(RawBacktrace, std::ostream& out) final {
    out << "\n\t\tNote: stacktrace not available without access to the "
           "original binary.\n";
  }
};

std::unique_ptr<StacktraceSymbolizer>
StacktraceSymbolizer::create(const std::optional<std::string>& binary_file_path,
                             const DumpFileHeader& dump_file_header) {
  if (binary_file_path.has_value()) {
    return do_create(*binary_file_path, dump_file_header);
  }
  return std::make_unique<NullStacktraceSymbolizer>();
}

}  // namespace syan
