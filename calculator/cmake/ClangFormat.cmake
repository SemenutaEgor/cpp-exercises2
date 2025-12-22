find_program(CLANG_FORMAT_EXE NAMES clang-format)

file(GLOB_RECURSE ALL_CXX_FILES
    ${CMAKE_SOURCE_DIR}/app/*.cpp
    ${CMAKE_SOURCE_DIR}/mathlib/*.cpp
    ${CMAKE_SOURCE_DIR}/mathlib/*.h
)

if (ALL_CXX_FILES)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE}
        -i
        -style=file
        ${ALL_CXX_FILES}
        COMMENT "Running clang-format"
    )
else()
    message(WARNING "No source files found for clang-format")
endif()