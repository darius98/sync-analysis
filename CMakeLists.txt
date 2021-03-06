cmake_minimum_required(VERSION 3.15)
project(sync_analysis)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SYNC_ANALYSIS_VERSION "1.0")

add_compile_options(-Wall -Wextra -Wpedantic -fno-omit-frame-pointer)

set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_INSTALL OFF)
add_subdirectory(third_party/benchmark)
add_subdirectory(third_party/mcga-cli)

add_compile_options(-fno-exceptions)

add_subdirectory(lib)
add_subdirectory(integration)
add_subdirectory(executable)
add_subdirectory(analyzers)
add_subdirectory(tests)
add_subdirectory(benchmarks)
