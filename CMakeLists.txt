cmake_minimum_required(VERSION 3.15)
project(ara_sm_demo LANGUAGES CXX)

option(BUILD_TESTS "Enable unit tests" ON)
option(COVERAGE "Enable coverage" OFF)

set(CMAKE_CXX_STANDARD 17)

if(COVERAGE)
    add_compile_options(-O0 -g --coverage -fprofile-arcs -ftest-coverage)
    add_link_options(--coverage)
endif()

add_library(ara_sm
    src/action_executor.cpp
    src/error_recovery.cpp
    src/state_machine.cpp
    src/transition_table.cpp
)

target_include_directories(ara_sm PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/config
)

# >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# DODAJ GOOGLETEST TUTAJ
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
add_subdirectory(external/googletest)
# <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/unit)
endif()
