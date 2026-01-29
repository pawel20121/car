cmake_minimum_required(VERSION 3.15)
project(ara_sm_demo LANGUAGES CXX)

option(BUILD_TESTS "Enable unit tests" ON)
option(COVERAGE "Enable coverage" OFF)

set(CMAKE_CXX_STANDARD 17)

# =====================================================================
# COVERAGE FLAGS (GCC / MinGW)
# =====================================================================
if(COVERAGE)
    message(STATUS "Enabling coverage flags (GCC/MinGW)")

    set(COVERAGE_COMPILE_FLAGS
        -O0
        -g
        -fprofile-arcs
        -ftest-coverage
    )

    set(COVERAGE_LINK_FLAGS
        -fprofile-arcs
        -ftest-coverage
    )
endif()

# =====================================================================
# LIBRARY
# =====================================================================
add_library(ara_sm
    src/action_executor.cpp
    src/error_recovery.cpp
    src/state_machine.cpp
    src/transition_table.cpp
    src/update_request_service.cpp
    config/static_config.cpp
    config/static_config_helpers.cpp
)

target_include_directories(ara_sm PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/ara/core
    ${CMAKE_SOURCE_DIR}/include/ara/sm
    ${CMAKE_SOURCE_DIR}/config
)

if(COVERAGE)
    target_compile_options(ara_sm PRIVATE ${COVERAGE_COMPILE_FLAGS})
    target_link_options(ara_sm    PRIVATE ${COVERAGE_LINK_FLAGS})
endif()

# =====================================================================
# GOOGLE TEST
# =====================================================================
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
add_subdirectory(external/googletest)


if(COVERAGE)
    target_compile_options(gtest PRIVATE ${COVERAGE_COMPILE_FLAGS})
    target_link_options(gtest    PRIVATE ${COVERAGE_LINK_FLAGS})

    target_compile_options(gtest_main PRIVATE ${COVERAGE_COMPILE_FLAGS})
    target_link_options(gtest_main    PRIVATE ${COVERAGE_LINK_FLAGS})
endif()

# =====================================================================
# UNIT TESTS
# =====================================================================
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/unit)
endif()

if(COVERAGE AND TARGET unit_tests)
    target_compile_options(unit_tests PRIVATE ${COVERAGE_COMPILE_FLAGS})
    target_link_options(unit_tests    PRIVATE ${COVERAGE_LINK_FLAGS})
endif()

# =====================================================================
# CPPCHECK
# =====================================================================
find_program(CPPCHECK_EXECUTABLE NAMES cppcheck)
if(CPPCHECK_EXECUTABLE)
    add_custom_target(cppcheck
        COMMAND ${CPPCHECK_EXECUTABLE}
                --enable=all --inconclusive --std=c++17 --force
                --suppress=missingIncludeSystem
                ${CMAKE_SOURCE_DIR}/src
                ${CMAKE_SOURCE_DIR}/include
                ${CMAKE_SOURCE_DIR}/include/ara/core
                ${CMAKE_SOURCE_DIR}/include/ara/sm
                ${CMAKE_SOURCE_DIR}/config
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running cppcheck static analysis"
    )
endif()

# =====================================================================
# GCOVR COVERAGE TARGET
# =====================================================================
find_program(GCOVR_EXECUTABLE NAMES gcovr)
if(GCOVR_EXECUTABLE)
    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target unit_tests
        COMMAND ${CMAKE_CTEST_COMMAND} --test-dir ${CMAKE_BINARY_DIR} --output-on-failure
        COMMAND ${GCOVR_EXECUTABLE} -r ${CMAKE_SOURCE_DIR} --txt -o ${CMAKE_SOURCE_DIR}/coverage_report.txt
        COMMAND ${GCOVR_EXECUTABLE} -r ${CMAKE_SOURCE_DIR} --html-details -o ${CMAKE_SOURCE_DIR}/coverage_report.html
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running tests and generating coverage reports (gcovr)"
    )
endif()
