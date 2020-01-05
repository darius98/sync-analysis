function(add_ill_formed_test NAME FILE)
    add_executable(${NAME}_bin ${FILE})
    target_link_libraries(${NAME}_bin PUBLIC cxx_sync_lib)
endfunction()

add_ill_formed_test(mutex_ordering ${CMAKE_CURRENT_SOURCE_DIR}/deadlock/mutex_ordering.cpp)
