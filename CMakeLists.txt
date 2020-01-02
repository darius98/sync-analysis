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
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/init.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/open_output_stream.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/sync_analysis.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/worker_thread.c
        )
target_include_directories(sync_analysis_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)
