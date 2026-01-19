add_executable(calculator_tests
    test_runner_inout.cpp
)

target_link_libraries(calculator_tests
    PRIVATE
        calculator_core
        GTest::gtest
        GTest::gtest_main
)

enable_warnings(calculator_tests)

include(GoogleTest)
gtest_discover_tests(calculator_tests)
