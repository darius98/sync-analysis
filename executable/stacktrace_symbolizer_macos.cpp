#include "stacktrace_symbolizer.hpp"

#include <iomanip>
#include <sstream>

namespace syan {

class StacktraceSymbolizerImpl: public StacktraceSymbolizer {
public:
  StacktraceSymbolizerImpl(const std::string& binary_file_path,
                           const DumpFileHeader& dump_file_header)
      : binary_file_path(binary_file_path),
        dump_file_header(dump_file_header) {}

  void symbolize_stacktrace(const Event& event, std::ostream& out) final {
    std::stringstream atos_command_builder;
    // TODO: Don't hardcode architecture here!
    atos_command_builder << "atos -o " << binary_file_path
                         << " -arch x86_64 -l 0x" << std::hex
                         << std::setfill('0') << std::setw(16)
                         << dump_file_header.program_load_addr + 8;
    for (const auto& pc : event.raw_backtrace()) {
      if (pc != 0) {
        atos_command_builder << " 0x" << std::hex << std::setfill('0')
                             << std::setw(16) << pc;
      }
    }
    out << "\t\t";
    auto atos_command = atos_command_builder.str();
    std::FILE* atos_process = popen(atos_command.c_str(), "r");
    char ch;
    while ((ch = std::fgetc(atos_process)) != EOF) {
      out << ch;
      if (ch == '\n') {
        out << "\t\t";
      }
    }
    pclose(atos_process);
  }

private:
  std::string binary_file_path;
  DumpFileHeader dump_file_header;
};

std::unique_ptr<StacktraceSymbolizer>
StacktraceSymbolizer::Create(const std::string& binary_file_path,
                             const DumpFileHeader& dump_file_header) {
  return std::make_unique<StacktraceSymbolizerImpl>(binary_file_path,
                                                    dump_file_header);
}

}  // namespace syan