add_library(cxxsync STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/cxxsync/exception.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cxxsync/mutex.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cxxsync/recursive_mutex.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cxxsync/rwlock.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cxxsync/thread.cpp
        )
target_include_directories(cxxsync PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cxxsync/include)
target_link_libraries(cxxsync PUBLIC pthread sync_analysis_lib)
install(TARGETS cxxsync DESTINATION lib)
install(DIRECTORY cxxsync/include DESTINATION .)
