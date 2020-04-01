add_custom_target(all_extensions COMMAND echo "All extensions built!")

macro(add_syan_extension NAME)
    add_library(syanext-${NAME} SHARED ${NAME}.cpp)
    target_include_directories(syanext-${NAME} PRIVATE ${PROJECT_SOURCE_DIR}/executable/include)
    target_compile_options(syanext-${NAME} PRIVATE -fvisibility=hidden)
    target_link_options(syanext-${NAME} PRIVATE -undefined dynamic_lookup)
    set_target_properties(syanext-${NAME} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/syan-ext)
    add_dependencies(all_extensions syanext-${NAME})
endmacro()

macro(add_syan_extensions)
    foreach (EXT ${ARGN})
        add_syan_extension(${EXT})
    endforeach ()
endmacro()