#ifndef SYNC_ANALYSIS_EXECUTABLE_SRC_FIND_EXTENSIONS_H_
#define SYNC_ANALYSIS_EXECUTABLE_SRC_FIND_EXTENSIONS_H_

#include <filesystem>
#include <string>
#include <vector>

#include "extension.hpp"
#include "options.hpp"

namespace syan {

std::vector<Extension> find_extensions(const Options& options);

}

#endif
