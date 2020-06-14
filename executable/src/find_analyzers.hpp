#ifndef SYNC_ANALYSIS_EXECUTABLE_SRC_FIND_ANALYZERS_H_
#define SYNC_ANALYSIS_EXECUTABLE_SRC_FIND_ANALYZERS_H_

#include <vector>

#include "analyzer.hpp"
#include "options.hpp"

namespace syan {

std::vector<Analyzer> find_analyzers(const Options& options);

}

#endif
