enable_testing()

add_custom_target(clear_coverage_data
        COMMAND find ${PROJECT_BINARY_DIR} -name "*.gcda" -print0 | xargs -0 rm)

add_executable(run_extension_test run_extension_test.cpp)
target_link_libraries(run_extension_test mcga_cli)

add_custom_target(tests.extensions
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "$extension.")
add_dependencies(tests.extensions clear_coverage_data run_extension_test sync_analysis all_extensions)

function(add_extension_test EXTENSION TYPE DESCRIPTION)
    set(TEST_NAME "extension.${EXTENSION}.${TYPE}.${DESCRIPTION}")
    set(TEST_DIR ${PROJECT_SOURCE_DIR}/.test-run/${TEST_NAME})
    file(MAKE_DIRECTORY ${TEST_DIR})
    add_executable(${TEST_NAME}.exe "${CMAKE_CURRENT_SOURCE_DIR}/${EXTENSION}/${TYPE}.${DESCRIPTION}.cpp")
    set_target_properties(${TEST_NAME}.exe PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${TEST_DIR}
            RUNTIME_OUTPUT_NAME test_binary)
    target_link_libraries(${TEST_NAME}.exe PUBLIC cxx_sync_lib)
    add_test(NAME ${TEST_NAME} COMMAND run_extension_test
            --test_binary=${TEST_DIR}/test_binary
            --sync_analysis_binary=$<TARGET_FILE:sync_analysis>
            --sync_analysis_extension_dir=${PROJECT_BINARY_DIR}/syan-ext
            --sync_analysis_extension_name=${EXTENSION}
            --sync_analysis_output_file=${TEST_DIR}/report.txt
            WORKING_DIRECTORY ${TEST_DIR}
            )
    if (TYPE STREQUAL "positive")
        set_tests_properties(${TEST_NAME} PROPERTIES
                WILL_FAIL TRUE)
    endif ()
    set_tests_properties(${TEST_NAME} PROPERTIES
            ENVIRONMENT UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1)
    add_dependencies(tests.extensions ${TEST_NAME}.exe)
endfunction()

add_extension_test(lock-shadow negative rwlock_rd_outside_shadow)
add_extension_test(lock-shadow negative sometimes_not_shadowed)
add_extension_test(lock-shadow negative single_mutex)
add_extension_test(lock-shadow positive mutex_and_rw_lock)
add_extension_test(lock-shadow positive three_locks)
add_extension_test(lock-shadow positive two_mutexes)
add_extension_test(mutex-lock-order negative three_mutexes_ordered_correctly)
#TODO: Fix this!
#add_extension_test(mutex-lock-order positive three_mutexes)
add_extension_test(mutex-lock-order positive two_mutexes)
add_extension_test(redundant-recursive-mutex negative rec_mutex_used_correctly)
add_extension_test(redundant-recursive-mutex negative unused_leaked_mutex)
add_extension_test(redundant-recursive-mutex negative unused_mutex)
add_extension_test(redundant-recursive-mutex positive on_mutex_destroyed)
add_extension_test(redundant-recursive-mutex positive on_mutex_leaked)
add_extension_test(redundant-rwlock negative used_as_both_rd_and_wr)
add_extension_test(redundant-rwlock positive used_only_as_rd_lock)
add_extension_test(redundant-rwlock positive used_only_as_wr_lock)
