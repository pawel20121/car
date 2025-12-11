# AUTOSAR Adaptive Platform — State Management Module
Professional Implementation • Unit Testing • Static Analysis • Coverage

## 1. Introduction

This repository provides a complete implementation of the **State Management** module for the **AUTOSAR Adaptive Platform**. The module manages both the execution lifecycle of Adaptive Applications and Function Group behavior by providing:

- Machine State Management
- Function Group State Management
- Validation and execution of state transitions
- Action List processing
- Interfacing with other Adaptive Platform services:
  - Execution Management
  - Network Management
  - Update & Configuration Management

The design follows the guidelines of **AUTOSAR Adaptive Platform SWS State Management** and emphasizes modularity, maintainability, and testability.

## 2. Key Features

- AUTOSAR-aligned state transition logic
- Clear separation of API, configuration, and implementation layers
- Mock-based Action Executor enabling high-quality unit testing
- Comprehensive unit tests using GoogleTest
- Automated static analysis:
  - cppcheck
  - GCC `-fanalyzer`
- Coverage reporting using gcov and gcovr
- CMake-based build system compatible with MinGW/GCC

## 3. Repository Structure

```
.
├── config/              # Static configuration for state transitions
├── external/            # External dependencies (GoogleTest via submodule)
├── include/             # Public API and module interfaces
├── scripts/             # Build, analysis, and coverage utility scripts
├── src/                 # Implementation of the State Management module
└── tests/               # Unit tests (GoogleTest)
```

## 4. External Dependencies

This project uses **GoogleTest** as a submodule.

Install or update it:

```bash
git submodule add https://github.com/google/googletest.git external/googletest
git submodule update --init --recursive
```

## 5. Build Instructions (MinGW + CMake)

### 5.1 Configure and build

```powershell
Remove-Item -Recurse -Force .\build\
cmake -B build -G "MinGW Makefiles" -DBUILD_TESTS=ON -DCOVERAGE=ON .
cmake --build build
```

### 5.2 Running Tests

```powershell
cd build
ctest --verbose
```

## 6. Static Analysis

A script is provided to run static analysis:

Tools:
- cppcheck
- GCC `-fanalyzer`

Execute:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\run_analysis.ps1
```

## 7. Code Coverage

Coverage is generated with **gcov** and **gcovr**, with GoogleTest excluded from reports.

Run:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\run_coverage.ps1
```

Generated reports:

- `coverage_report.txt`
- `coverage_report.html`

## 8. Project Purpose (Student Project)

This implementation was developed as part of a software engineering learning project. The goals included:

- Understanding AUTOSAR Adaptive Platform architecture
- Implementing a real subsystem: State Management
- Developing according to SWE.3 / SWE.4 maturity practices
- Building a full development toolchain:
  - modular implementation
  - unit testing
  - static analysis
  - coverage reporting

## 9. AUTOSAR Specification Reference

This project references:

- **AUTOSAR Adaptive Platform R21-11**
  (Selected requirements from *SWS State Management*)

## 10. License

This project is intended for educational and research use.
For redistribution or commercial use, please contact the author.

