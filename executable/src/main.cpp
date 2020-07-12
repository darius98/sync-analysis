#include <iostream>

#include "debug.hpp"
#include "find_analyzers.hpp"
#include "options.hpp"
#include "run_analysis.hpp"

int main(int argc, char** argv) {
  syan::Options options(argc, argv);
  syan::debugging::set_debug_enabled(options.debug_mode);
  DOUT << "Loaded command line options";

  DOUT << "Loading analyzers...";
  auto analyzers = syan::find_analyzers(options);
  DOUT << "Loaded analyzers:";
  for (const auto& analyzer : analyzers) {
    DOUT << "\t" << analyzer.get_name() << "\t" << analyzer.get_dso_file_path();
  }
  DOUT << "Running analysis...";
  int status = syan::run_analysis(options, std::move(analyzers));
  DOUT << "Analysis done. Exit status = " << status;
  return status;
}
