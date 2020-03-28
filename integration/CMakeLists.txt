add_library(cxx_sync_lib STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/cxx_sync/exception.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cxx_sync/mutex.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cxx_sync/recursive_mutex.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cxx_sync/rwlock.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cxx_sync/thread.cpp
        )
target_include_directories(cxx_sync_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cxx_sync_lib PUBLIC pthread sync_analysis_lib)
