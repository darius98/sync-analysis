set(sync_analysis_PLATFORM_SPECIFIC_SRC)
if (APPLE)
    set(sync_analysis_PLATFORM_SPECIFIC_SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/stacktrace_symbolizer_macos.cpp)
endif ()
add_executable(sync_analysis
        ${CMAKE_CURRENT_SOURCE_DIR}/src/analyzer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/database.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/debug.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/environment.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/event.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/event_file_reader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/find_analyzers.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/options.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/report.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/run_analysis.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/stacktrace_symbolizer.cpp
        ${sync_analysis_PLATFORM_SPECIFIC_SRC}
        )
target_include_directories(sync_analysis
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../
        SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/mcga-cli/include
        )
target_link_libraries(sync_analysis PRIVATE mcga_cli PRIVATE sync_analysis_lib_options)
target_compile_definitions(sync_analysis PUBLIC
        -DSYNC_ANALYSIS_IS_IN_EXECUTABLE
        -DSYNC_ANALYSIS_VERSION="${SYNC_ANALYSIS_VERSION}"
        )
target_link_options(sync_analysis PUBLIC -rdynamic)
install(TARGETS sync_analysis DESTINATION bin)
install(DIRECTORY include DESTINATION .)
