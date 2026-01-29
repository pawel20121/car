cmake_minimum_required(VERSION 3.15)

add_executable(unit_tests
    test_state_machine.cpp
    test_transition_table.cpp
    test_error_recovery.cpp
    test_update_request_service.cpp
    test_static_config.cpp
    test_action_executor.cpp
    
)


target_link_libraries(unit_tests
    ara_sm
    # gmock
    gtest
    gtest_main
)

enable_testing()
add_test(NAME unit_tests COMMAND unit_tests)
