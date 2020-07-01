add_library(cxxsync SHARED
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

add_library(syan_pthread_shim SHARED ${CMAKE_CURRENT_SOURCE_DIR}/pthread_shim.c)
target_link_libraries(syan_pthread_shim PUBLIC sync_analysis_lib)
install(TARGETS syan_pthread_shim DESTINATION lib)
