set(sync_analysis_PLATFORM_SPECIFIC_SRC)
if (APPLE)
    set(sync_analysis_PLATFORM_SPECIFIC_SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/stacktrace_symbolizer_macos.cpp)
else ()
    set(sync_analysis_PLATFORM_SPECIFIC_SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/stacktrace_symbolizer_linux.cpp)
endif ()
add_library(syan_analyzer_api SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/database.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/event.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/analyzer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/environment.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/global_environment.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/report.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/stacktrace_symbolizer.cpp
        ${sync_analysis_PLATFORM_SPECIFIC_SRC}
        )
target_include_directories(syan_analyzer_api
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
        SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/mcga-cli/include
        )
target_link_libraries(syan_analyzer_api PUBLIC mcga_cli PUBLIC sync_analysis_lib_options)
target_compile_definitions(syan_analyzer_api PUBLIC -DSYNC_ANALYSIS_VERSION="${SYNC_ANALYSIS_VERSION}")

add_library(sync_analysis_event_file_reader_helperlib
        ${CMAKE_CURRENT_SOURCE_DIR}/src/event_file_reader.cpp)
target_link_libraries(sync_analysis_event_file_reader_helperlib PUBLIC syan_analyzer_api)

add_executable(sync_analysis
        ${CMAKE_CURRENT_SOURCE_DIR}/src/debug.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/event_file_reader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/find_analyzers.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/options.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/run_analysis.cpp
        )
target_link_libraries(sync_analysis syan_analyzer_api sync_analysis_event_file_reader_helperlib)

install(TARGETS sync_analysis DESTINATION bin)
install(TARGETS syan_analyzer_api DESTINATION lib)
install(DIRECTORY include DESTINATION .)
