cmake_minimum_required(VERSION 3.16)

project(calculator VERSION 1.0 LANGUAGES  CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/ClangTidy.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/ClangFormat.cmake)

include(FetchContent)

FetchContent_Declare(
    mathlib
    GIT_REPOSITORY https://github.com/SemenutaEgor/mathlib.git
    GIT_TAG v1.0.1
)

FetchContent_MakeAvailable(mathlib)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(nlohmann_json)

add_library(calculator_core
    src/core/Runner.cpp
    src/core/Parser.cpp
    src/core/Checker.cpp
    src/core/Calculator.cpp
    src/core/Printer.cpp
    src/logging/Logger.cpp
    src/storage/CachedExecutor.cpp
)


target_include_directories(calculator_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(calculator_core
    PRIVATE
        mathlib
        nlohmann_json::nlohmann_json
)

enable_clang_tidy(calculator_core)
enable_warnings(calculator_core)

add_executable(calculator
    src/main.cpp
)

target_link_libraries(calculator
    PRIVATE
        calculator_core
)

enable_clang_tidy(calculator)
enable_warnings(calculator)

install(TARGETS calculator
    RUNTIME DESTINATION bin
)

configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/Uninstall.cmake
    ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
    IMMEDIATE
    @ONLY
)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(tests)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.13.0
)

FetchContent_MakeAvailable(spdlog)

target_link_libraries(calculator_core
    PRIVATE
        spdlog::spdlog
)
