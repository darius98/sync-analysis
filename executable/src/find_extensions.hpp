#ifndef SYNC_ANALYSIS_EXECUTABLE_SRC_FIND_EXTENSIONS_H_
#define SYNC_ANALYSIS_EXECUTABLE_SRC_FIND_EXTENSIONS_H_

#include <filesystem>
#include <vector>

#include "extension.hpp"

namespace syan {

std::vector<Extension>
find_extensions(const std::vector<std::filesystem::path>& directories);

}

#endif
