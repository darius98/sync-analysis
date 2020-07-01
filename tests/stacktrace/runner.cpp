#include <cstdlib>
#include <sys/wait.h>

#include <iostream>

#include "executable/src/event_file_reader.hpp"
#include "executable/src/stacktrace_symbolizer.hpp"

using syan::DumpFileHeader;
using syan::EventFileReader;
using syan::StacktraceSymbolizer;

int main() {
  int exit_code = std::system("./event_emitter");
  if (WEXITSTATUS(exit_code) != 0) {
    return 1;
  }

  EventFileReader reader("sync_analysis.dump");
  DumpFileHeader header(std::move(reader.release_header()));
  auto symbolizer = StacktraceSymbolizer::create("./event_emitter", header);

  while (!reader.done()) {
    auto event = reader.read();
    if (event != nullptr) {
      symbolizer->symbolize_stacktrace(event.raw_backtrace(), std::cout);
      std::cout << "\n-------\n";
    }
  }

  return 0;
}
