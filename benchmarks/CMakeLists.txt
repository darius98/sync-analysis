add_executable(sync_analysis_benchmarks
        latency_benchmark.cpp
        throughput_benchmark.cpp)
target_link_libraries(sync_analysis_benchmarks benchmark benchmark_main sync_analysis_lib)
