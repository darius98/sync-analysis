function(add_syan_extension NAME)
    add_library(syanext-${NAME} SHARED ${ARGN})
    target_include_directories(syanext-${NAME} PRIVATE ${PROJECT_SOURCE_DIR}/executable/include)
    target_compile_options(syanext-${NAME} PRIVATE -fvisibility=hidden)
    target_link_options(syanext-${NAME} PRIVATE -undefined dynamic_lookup)
    set_target_properties(syanext-${NAME} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/syan-ext)
endfunction()
