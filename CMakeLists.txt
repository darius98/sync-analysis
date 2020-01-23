cmake_minimum_required(VERSION 3.15)
project(sync_analysis)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(SYNC_ANALYSIS_VERSION "1.0")

add_library(sync_analysis_lib STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/backtrace.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/buffer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/dump_file_header.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/event_time.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/global_buffer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/handle_event.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/init.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/sync_analysis.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/worker_thread.c
        )
target_include_directories(sync_analysis_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)
target_link_libraries(sync_analysis_lib PUBLIC pthread)
target_compile_definitions(sync_analysis_lib PRIVATE UNW_LOCAL_ONLY)
if (APPLE)
    target_compile_definitions(sync_analysis_lib PUBLIC SYNC_ANALYSIS_IS_MAC_OS_X)
elseif (LINUX)
    target_compile_definitions(sync_analysis_lib PUBLIC SYNC_ANALYSIS_IS_LINUX)
else ()
    message(FATAL "Target platform not supported.")
endif ()

add_library(cxx_sync_lib STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/integration/cxx_sync/exception.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/integration/cxx_sync/mutex.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/integration/cxx_sync/recursive_mutex.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/integration/cxx_sync/rwlock.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/integration/cxx_sync/thread.cpp
        )
target_include_directories(cxx_sync_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/integration)
target_link_libraries(cxx_sync_lib PUBLIC pthread sync_analysis_lib)

add_executable(sync_analysis
        ${CMAKE_CURRENT_SOURCE_DIR}/executable/active_objects_db.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/executable/environment.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/executable/event.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/executable/event_file_reader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/executable/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/executable/report.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/executable/checks/list.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/executable/checks/mutex_lock_order.cpp
        )
target_link_libraries(sync_analysis PUBLIC sync_analysis_lib)
target_include_directories(sync_analysis
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/mcga-cli/include
        )
target_compile_definitions(sync_analysis PUBLIC
        -DSYNC_ANALYSIS_IS_IN_EXECUTABLE
        -DSYNC_ANALYSIS_VERSION="${SYNC_ANALYSIS_VERSION}"
        )

add_subdirectory(tests)
