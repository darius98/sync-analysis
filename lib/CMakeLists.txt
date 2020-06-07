add_library(sync_analysis_lib STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/backtrace.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/buffer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/dump_file_header.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/event_time.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/global_buffer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/handle_event.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/init.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sync_analysis.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/worker_thread.c
        )
target_include_directories(sync_analysis_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(sync_analysis_lib PUBLIC pthread)
target_compile_definitions(sync_analysis_lib PRIVATE UNW_LOCAL_ONLY)
set_target_properties(sync_analysis_lib PROPERTIES OUTPUT_NAME sync_analysis)
if (APPLE)
    target_compile_definitions(sync_analysis_lib PUBLIC SYNC_ANALYSIS_IS_MAC_OS_X)
elseif (LINUX)
    target_compile_definitions(sync_analysis_lib PUBLIC SYNC_ANALYSIS_IS_LINUX)
else ()
    message(FATAL "Target platform not supported.")
endif ()
install(TARGETS sync_analysis_lib DESTINATION lib)
install(DIRECTORY include DESTINATION .)
