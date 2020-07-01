set(TEST_DIR ${PROJECT_SOURCE_DIR}/.test-run/stacktrace)
file(MAKE_DIRECTORY ${TEST_DIR})

add_executable(test.stacktrace.event_emitter EXCLUDE_FROM_ALL event_emitter.cpp)
target_link_libraries(test.stacktrace.event_emitter PUBLIC sync_analysis_lib)
set_target_properties(test.stacktrace.event_emitter PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TEST_DIR}
        RUNTIME_OUTPUT_NAME event_emitter)

add_executable(test.stacktrace.runner EXCLUDE_FROM_ALL runner.cpp)
target_link_libraries(test.stacktrace.runner PUBLIC sync_analysis_executable_lib sync_analysis_lib_options)
set_target_properties(test.stacktrace.runner PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TEST_DIR}
        RUNTIME_OUTPUT_NAME runner)
add_dependencies(test.stacktrace.runner test.stacktrace.event_emitter)
