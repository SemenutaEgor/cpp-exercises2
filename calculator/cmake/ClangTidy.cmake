option(USE_CLANG_TIDY "Enable clang-tidy checks" ON)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)

if(CLANG_TIDY_EXE AND USE_CLANG_TIDY)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    function(enable_clang_tidy target)
        if(TARGET ${target})
            set_target_properties(${target} PROPERTIES
                CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-warnings-as-errors=*"
            )
        endif()
    endfunction()
else()
    message(STATUS "clang-tidy not used")
    function(enable_clang_tidy target)
    endfunction()
endif()
