enable_testing()

add_custom_target(clear_coverage_data
        COMMAND find ${PROJECT_BINARY_DIR} -name "*.gcda" -print0 | xargs -0 rm)

add_executable(run_analyzer_test run_analyzer_test.cpp)
target_link_libraries(run_analyzer_test mcga_cli)

add_custom_target(tests.analyzers
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "$analyzer.")
add_dependencies(tests.analyzers clear_coverage_data run_analyzer_test sync_analysis all_analyzers)

function(add_analyzer_test ANALYZER TYPE DESCRIPTION)
    set(TEST_NAME "analyzer.${ANALYZER}.${TYPE}.${DESCRIPTION}")
    set(TEST_DIR ${PROJECT_SOURCE_DIR}/.test-run/${TEST_NAME})
    file(MAKE_DIRECTORY ${TEST_DIR})
    add_executable(${TEST_NAME}.exe "${CMAKE_CURRENT_SOURCE_DIR}/${ANALYZER}/${TYPE}.${DESCRIPTION}.cpp")
    set_target_properties(${TEST_NAME}.exe PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${TEST_DIR}
            RUNTIME_OUTPUT_NAME test_binary)
    target_link_libraries(${TEST_NAME}.exe PUBLIC cxx_sync_lib)
    add_test(NAME ${TEST_NAME} COMMAND run_analyzer_test
            --test_binary=${TEST_DIR}/test_binary
            --sync_analysis_binary=$<TARGET_FILE:sync_analysis>
            --sync_analysis_analyzer_dir=${PROJECT_BINARY_DIR}/syan-analyzers
            --sync_analysis_analyzer_name=${ANALYZER}
            --sync_analysis_output_file=${TEST_DIR}/report.txt
            WORKING_DIRECTORY ${TEST_DIR}
            )
    if (TYPE STREQUAL "positive")
        set_tests_properties(${TEST_NAME} PROPERTIES
                WILL_FAIL TRUE)
    endif ()
    set_tests_properties(${TEST_NAME} PROPERTIES
            ENVIRONMENT UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1)
    add_dependencies(tests.analyzers ${TEST_NAME}.exe)
endfunction()

add_analyzer_test(lock-shadow negative rwlock_rd_outside_shadow)
add_analyzer_test(lock-shadow negative sometimes_not_shadowed)
add_analyzer_test(lock-shadow negative single_mutex)
add_analyzer_test(lock-shadow positive mutex_and_rw_lock)
add_analyzer_test(lock-shadow positive three_locks)
add_analyzer_test(lock-shadow positive two_mutexes)
add_analyzer_test(mutex-lock-order negative three_mutexes_ordered_correctly)
#TODO: Fix this!
#add_analyzer_test(mutex-lock-order positive three_mutexes)
add_analyzer_test(mutex-lock-order positive two_mutexes)
add_analyzer_test(redundant-recursive-mutex negative rec_mutex_used_correctly)
add_analyzer_test(redundant-recursive-mutex negative unused_leaked_mutex)
add_analyzer_test(redundant-recursive-mutex negative unused_mutex)
add_analyzer_test(redundant-recursive-mutex positive on_mutex_destroyed)
add_analyzer_test(redundant-recursive-mutex positive on_mutex_leaked)
add_analyzer_test(redundant-rwlock negative used_as_both_rd_and_wr)
add_analyzer_test(redundant-rwlock positive used_only_as_rd_lock)
add_analyzer_test(redundant-rwlock positive used_only_as_wr_lock)
add_analyzer_test(useless-lock negative mutex_lock)
add_analyzer_test(useless-lock negative mutex_failed_try_lock)
add_analyzer_test(useless-lock negative rwlock_rd_lock)
add_analyzer_test(useless-lock positive mutex_unused)
add_analyzer_test(useless-lock positive mutex_single_thread_lock)
add_analyzer_test(useless-lock positive mutex_single_thread_try_lock)
add_analyzer_test(useless-lock positive rec_mutex_unused)
add_analyzer_test(useless-lock positive rec_mutex_single_thread)
add_analyzer_test(useless-lock positive rwlock_unused)
add_analyzer_test(useless-lock positive rwlock_single_thread_rd_lock)
add_analyzer_test(useless-lock positive rwlock_single_thread_try_rd_lock)
add_analyzer_test(useless-lock positive rwlock_single_thread_wr_lock)
add_analyzer_test(useless-lock positive rwlock_single_thread_try_wr_lock)
