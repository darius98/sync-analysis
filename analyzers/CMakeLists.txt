add_custom_target(all_analyzers COMMAND echo "All analyzers built!")

macro(add_syan_analyzer NAME)
    add_library(syan-analyzer-${NAME} SHARED ${NAME}.cpp)
    target_include_directories(syan-analyzer-${NAME} PRIVATE ${PROJECT_SOURCE_DIR}/executable/include)
    target_compile_options(syan-analyzer-${NAME} PRIVATE -fvisibility=hidden)
    target_link_options(syan-analyzer-${NAME} PRIVATE -undefined dynamic_lookup)
    set_target_properties(syan-analyzer-${NAME} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/syan-analyzers)
    add_dependencies(all_analyzers syan-analyzer-${NAME})
    install(TARGETS syan-analyzer-${NAME} DESTINATION syan-analyzers)
endmacro()

macro(add_syan_analyzers)
    foreach (ANALYZER ${ARGN})
        add_syan_analyzer(${ANALYZER})
    endforeach ()
endmacro()

add_syan_analyzers(
        lock-shadow
        mutex-lock-order
        redundant-recursive-mutex
        redundant-rwlock
        useless-lock
)
