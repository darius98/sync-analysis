cmake_minimum_required(VERSION 3.15)
project(sync_analysis)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_library(sync_analysis_lib STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/backtrace.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/buffer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/event_time.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/global_buffer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/handle_event.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/init.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/sync_analysis.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/worker_thread.c
        )
target_include_directories(sync_analysis_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)
target_link_libraries(sync_analysis_lib PUBLIC pthread)
target_compile_definitions(sync_analysis_lib PRIVATE UNW_LOCAL_ONLY)

add_library(cxx_sync_lib STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/integration/cxx_sync/exception.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/integration/cxx_sync/mutex.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/integration/cxx_sync/recursive_mutex.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/integration/cxx_sync/rwlock.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/integration/cxx_sync/thread.cpp
        )
target_include_directories(cxx_sync_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/integration)
target_link_libraries(cxx_sync_lib PUBLIC pthread sync_analysis_lib)
