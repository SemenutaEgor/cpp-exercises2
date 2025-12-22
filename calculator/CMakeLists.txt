cmake_minimum_required(VERSION 3.16)

project(calculator VERSION 1.0 LANGUAGES  CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY
        ${CLANG_TIDY_EXE};
        -warnings-as-errors=*
    )
endif()

add_library(mathlib STATIC
    mathlib/mathlib.cpp
)

target_include_directories(mathlib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR/mathlib}
)

add_executable(calculator
    app/main.cpp
)

target_link_libraries(calculator
    PRIVATE
        mathlib
)

install(TARGETS calculator
    RUNTIME DESTINATION bin
)

install(TARGETS mathlib
    ARCHIVE DESTINATION lib
)

install(FILES mathlib/mathlib.h
    DESTINATION include
)

include(cmake/ClangFormat.cmake)

configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/Uninstall.cmake
    ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
    IMMEDIATE
    @ONLY
)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
)