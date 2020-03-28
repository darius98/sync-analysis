cmake_minimum_required(VERSION 3.15)
project(sync_analysis)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(SYNC_ANALYSIS_VERSION "1.0")

add_compile_options(-Werror -Wall -Wextra -Wpedantic)

include(cmake/SyanExtension.cmake)

add_subdirectory(lib)
add_subdirectory(integration)
add_subdirectory(executable)
add_subdirectory(extensions)
add_subdirectory(tests)
