# AUTOSAR Adaptive Platform State Management Implementation

## Description

This project implements the **State Management** module for the **AUTOSAR Adaptive Platform**. The module is responsible for managing the states of the system, including state transitions for both **Machine States** and **Function Group States**. It also handles interactions with other key components of the platform, such as **Update and Configuration Management**, **Network Management**, and **Execution Management**.

The **State Management** module is designed to provide flexible, configurable management of system states, following the AUTOSAR standards for Adaptive Systems. The module includes the implementation of state transitions, action lists for each state, and interfaces for integration with other AUTOSAR services.


incluede in maun directory
git submodule add https://github.com/google/googletest.git external/googletest

build
Remove-Item -Recurse -Force .\build\
cmake -B build -G "MinGW Makefiles" -DBUILD_TESTS=ON -DCOVERAGE=ON .
cmake --build build


Możesz teraz uruchomić testy:

.\build\tests\unit\unit_tests.exe


lub (zalecane, bo integruje się z CTest):

cd build
ctest --verbose

Co masz teraz gotowe

Kompletny proces kompilacji (MinGW)

Integrację z GoogleTest

Automatyczną rejestrację testów dla CTest

Działające testy jednostkowe pokrywające:

przejścia stanów

niewłaściwe żądania

recovery po błędzie

blokadę update