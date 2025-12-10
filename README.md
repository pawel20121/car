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


Skrypt PowerShell (zawiera cppcheck i gcc fanalyzer)
powershell -ExecutionPolicy Bypass -File .\scripts\run_analysis.ps1


Skrypt PowerShell do wykonania coverage i wygenerowania raportów
powershell -ExecutionPolicy Bypass -File .\scripts\run_coverage.ps1
powershell -ExecutionPolicy Bypass -File .\scripts\run_coverage_gcov.ps1


Co masz teraz gotowe:
Kompletny proces kompilacji (MinGW)
Integrację z GoogleTest
Automatyczną rejestrację testów dla CTest
Działające testy jednostkowe pokrywające:
przejścia stanów
niewłaściwe żądania
recovery po błędzie
blokadę update




Jeśli chcesz gMock, wykonaj:

cd external
rm -Recurse -Force googletest
git clone --branch release-1.12.1 https://github.com/google/googletest.git
cd ..
Remove-Item -Recurse -Force build
cmake -B build -G "MinGW Makefiles" -DBUILD_TESTS=ON -DCOVERAGE=ON .
cmake --build build



CMake — dodatkowe targety
cmake --build build --target cppcheck
cmake --build build --target coverage



# AUTOSAR Adaptive — State Management (student project)

## Cel projektu
Implementacja modułu State Management (Adaptive Platform) na poziomie SWE.3 / SWE.4.
Zawiera: implementację, testy jednostkowe (GoogleTest), analizę statyczną (cppcheck, GCC -fanalyzer), oraz raport pokrycia (gcovr).

## Użyte specyfikacje
- AUTOSAR Adaptive Platform R21-11 (użyte elementy SWS State Management)

## Budowanie (Windows + MinGW)
1. Otwórz PowerShell
2. `cmake -B build -G "MinGW Makefiles" -DBUILD_TESTS=ON -DCOVERAGE=ON .`
3. `cmake --build build`
4. `cd build && ctest --verbose`

## Analiza statyczna
`powershell .\scripts\run_analysis.ps1`

## Pokrycie (coverage)
`powershell .\scripts\run_coverage.ps1`

## Testy jednostkowe
`cd build && ctest --verbose` lub `.\build\tests\unit\unit_tests.exe`

## Raporty
- `cppcheck_report.txt`
- `gcc_fanalyzer_report.txt`
- `coverage_report.txt`
- `coverage_report.html`


gMock vs manual mock — zalecenie

Na Twoim GCC 15 rekomenduję manualny mock (który dostarczyłem). Jest prosty, niezawodny i nie zależy od wersji googletest.

Jeżeli chcesz gMock, sklonuj googletest w wersji release-1.12.1 i zbuduj ponownie — dostarczę następnie przykładowe testy z użyciem gmock.