enable_testing()

add_custom_target(clear_coverage_data
        COMMAND find ${PROJECT_BINARY_DIR} -name "*.gcda" -print0 | xargs -0 rm)

add_executable(run_extension_test run_extension_test.cpp)
target_link_libraries(run_extension_test mcga_cli)

add_custom_target(tests.extensions
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "$extension.")
add_dependencies(tests.extensions clear_coverage_data run_extension_test sync_analysis all_extensions)

function(add_extension_test EXTENSION TYPE DESCRIPTION)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME})
    set(TEST_NAME "extension.${EXTENSION}.${TYPE}.${DESCRIPTION}")
    add_executable(${TEST_NAME}.exe "${CMAKE_CURRENT_SOURCE_DIR}/${EXTENSION}/${TYPE}.${DESCRIPTION}.cpp")
    set_target_properties(${TEST_NAME}.exe PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}
            RUNTIME_OUTPUT_NAME test_binary)
    target_link_libraries(${TEST_NAME}.exe PUBLIC cxx_sync_lib)
    add_test(NAME ${TEST_NAME} COMMAND run_extension_test
            --test_binary=$<TARGET_FILE:${TEST_NAME}.exe>
            --sync_analysis_binary=$<TARGET_FILE:sync_analysis>
            --sync_analysis_extension_dir=${PROJECT_BINARY_DIR}/syan-ext
            --extension=${EXTENSION}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}
            )
    if (TYPE STREQUAL "positive")
        set_tests_properties(${TEST_NAME} PROPERTIES WILL_FAIL TRUE)
    endif ()
    add_dependencies(tests.extensions ${TEST_NAME}.exe)
endfunction()

add_extension_test(mutex-lock-order positive two_mutexes)
#TODO: Fix this!
#add_extension_test(mutex-lock-order positive three_mutexes)
add_extension_test(mutex-lock-order negative three_mutexes_ordered_correctly)
add_extension_test(redundant-recursive-mutex positive on_mutex_destroyed)
add_extension_test(redundant-recursive-mutex positive on_mutex_leaked)
add_extension_test(redundant-recursive-mutex negative rec_mutex_used_correctly)
add_extension_test(redundant-recursive-mutex negative unused_leaked_mutex)
add_extension_test(redundant-recursive-mutex negative unused_mutex)
