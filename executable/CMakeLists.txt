set(sync_analysis_PLATFORM_SPECIFIC_SRC)
if (APPLE)
    set(sync_analysis_PLATFORM_SPECIFIC_SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/stacktrace_symbolizer_macos.cpp)
else ()
    set(sync_analysis_PLATFORM_SPECIFIC_SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/stacktrace_symbolizer_linux.cpp)
endif ()
add_library(sync_analysis_executable_lib
        ${CMAKE_CURRENT_SOURCE_DIR}/src/analyzer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/database.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/debug.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/environment.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/event.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/event_file_reader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/find_analyzers.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/options.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/report.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/run_analysis.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/stacktrace_symbolizer.cpp
        ${sync_analysis_PLATFORM_SPECIFIC_SRC}
        )
target_include_directories(sync_analysis_executable_lib
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
        SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/mcga-cli/include
        )
target_link_libraries(sync_analysis_executable_lib PRIVATE mcga_cli PRIVATE sync_analysis_lib_options PRIVATE dl)
target_compile_definitions(sync_analysis_executable_lib PUBLIC
        -DSYNC_ANALYSIS_IS_IN_EXECUTABLE
        -DSYNC_ANALYSIS_VERSION="${SYNC_ANALYSIS_VERSION}"
        )

add_executable(sync_analysis
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        )
target_link_libraries(sync_analysis sync_analysis_executable_lib mcga_cli sync_analysis_lib_options)
target_link_options(sync_analysis PUBLIC -rdynamic)

install(TARGETS sync_analysis DESTINATION bin)
install(DIRECTORY include DESTINATION .)
