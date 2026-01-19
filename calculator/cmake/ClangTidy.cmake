find_program(CLANG_TIDY_EXE NAMES clang-tidy)

if(CLANG_TIDY_EXE)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    function(enable_clang_tidy target)
        if(TARGET ${target})
            set_target_properties(${target} PROPERTIES
                CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-warnings-as-errors=*"
            )
        endif()
    endfunction()
else()
    message(STATUS "clang-tidy not found")
    function(enable_clang_tidy target)
    endfunction()
endif()
