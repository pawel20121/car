cmake_minimum_required(VERSION 3.15)
project(ara_sm_demo LANGUAGES CXX)

option(BUILD_TESTS "Enable unit tests" ON)
option(COVERAGE "Enable coverage" OFF)

set(CMAKE_CXX_STANDARD 17)

# =====================================================================
# COVERAGE FLAGS (MinGW / GCC)
# =====================================================================
if(COVERAGE)
    message(STATUS "Enabling coverage flags (MinGW/GCC)")

    # Wszystkie targety muszą dostać flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -O0 -g -fprofile-arcs -ftest-coverage")

    # Linkowanie wymagane dla gcov
    set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")
endif()

# =====================================================================
# LIBRARY
# =====================================================================
add_library(ara_sm
    src/action_executor.cpp
    src/error_recovery.cpp
    src/state_machine.cpp
    src/transition_table.cpp
    config/static_config.cpp
)

target_include_directories(ara_sm PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/config
)

# =====================================================================
# GOOGLE TEST
# =====================================================================
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
add_subdirectory(external/googletest)

# Wymuszamy coverage także na googletest jeśli COVERAGE=ON
if(COVERAGE)
    target_compile_options(gtest PRIVATE -O0 -g -fprofile-arcs -ftest-coverage)
    target_link_libraries(gtest PRIVATE gcov)

    target_compile_options(gtest_main PRIVATE -O0 -g -fprofile-arcs -ftest-coverage)
    target_link_libraries(gtest_main PRIVATE gcov)
endif()

# =====================================================================
# UNIT TESTS
# =====================================================================
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/unit)
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
                ${CMAKE_SOURCE_DIR}/config
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running cppcheck static analysis"
    )
endif()

# =====================================================================
# GCOVR COVERAGE TARGET (działa gdy zainstalowany gcovr)
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
