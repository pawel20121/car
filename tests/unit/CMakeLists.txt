cmake_minimum_required(VERSION 3.15)

add_executable(unit_tests
    test_state_machine.cpp
)

target_link_libraries(unit_tests
    ara_sm
    gtest
    gtest_main
)

enable_testing()
add_test(NAME unit_tests COMMAND unit_tests)
