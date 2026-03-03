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
add_test(NAME calculator_tests COMMAND calculator_tests)

add_executable(test_service_tcp
    test_service_tcp.cpp
)

target_include_directories(test_service_tcp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(test_service_tcp
    PRIVATE
        GTest::gtest
        GTest::gtest_main
)

enable_warnings(test_service_tcp)

add_test(NAME test_service_tcp COMMAND test_service_tcp)
set_tests_properties(test_service_tcp PROPERTIES WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

find_program(VALGRIND_EXECUTABLE valgrind)

string(FIND "${CMAKE_CXX_FLAGS}" "-fsanitize=thread" TSAN_FLAG_POS)
if(TSAN_FLAG_POS EQUAL -1)
  find_program(VALGRIND_EXECUTABLE valgrind)
  if(VALGRIND_EXECUTABLE)
    add_test(
      NAME calculator_valgrind
      COMMAND bash -lc
        "${VALGRIND_EXECUTABLE} --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=definite,indirect --error-exitcode=1 $<TARGET_FILE:calculator> --stdin < ${CMAKE_CURRENT_SOURCE_DIR}/valgrind_input.json"
    )
    set_tests_properties(calculator_valgrind PROPERTIES TIMEOUT 180)
  endif()
endif()


