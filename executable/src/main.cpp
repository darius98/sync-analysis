#include <iostream>

#include "debug.hpp"
#include "find_extensions.hpp"
#include "options.hpp"
#include "run_analysis.hpp"

int main(int argc, char** argv) {
  syan::Options options(argc, argv);
  syan::debugging::set_debug_enabled(options.debug_mode);
  DOUT << "Loaded command line options";

  DOUT << "Loading extensions...";
  auto extensions = syan::find_extensions(options);
  DOUT << "Loaded extensions:";
  for (const auto& extension : extensions) {
    DOUT << "\t" << extension.get_name();
  }
  DOUT << "Running analysis...";
  int status = syan::run_analysis(options, std::move(extensions));
  DOUT << "Analysis done. Exit status = " << status;
  return status;
}
