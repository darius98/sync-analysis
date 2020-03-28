function(add_syan_extension NAME)
    add_library(syan_ext_${NAME} SHARED ${ARGN})
    target_include_directories(syan_ext_${NAME} PRIVATE ${PROJECT_SOURCE_DIR}/executable/include)
    target_compile_options(syan_ext_${NAME} PRIVATE -fvisibility=hidden)
    target_link_options(syan_ext_${NAME} PRIVATE -undefined dynamic_lookup)
    set_target_properties(syan_ext_${NAME} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/syan-ext)
endfunction()
