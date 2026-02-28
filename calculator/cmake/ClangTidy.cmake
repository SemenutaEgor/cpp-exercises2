find_program(CLANG_TIDY_EXE NAMES clang-tidy)

option(USE_CLANG_TIDY "Enable clang-tidy" ON)
option(CLANG_TIDY_WERROR "Treat clang-tidy warnings as errors" OFF)

if(CLANG_TIDY_EXE AND USE_CLANG_TIDY)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")

    function(enable_clang_tidy target)
        if(TARGET ${target})
            set(_tidy_cmd "${CLANG_TIDY_EXE}")

            if(CLANG_TIDY_WERROR)
                list(APPEND _tidy_cmd "-warnings-as-errors=*")
            endif()

            list(APPEND _tidy_cmd "--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")

            set_target_properties(${target} PROPERTIES
                CXX_CLANG_TIDY "${_tidy_cmd}"
            )
        endif()
    endfunction()
else()
    message(STATUS "clang-tidy not used")
    function(enable_clang_tidy target)
    endfunction()
endif()
