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

---

## 5. Required Tooling

This section lists all tools required to build the project, run tests, perform static analysis, and generate coverage reports.

### 5.1 Build Environment
- **CMake ≥ 3.15** — required to generate the build system
- **GCC / MinGW with C++17 support** — required for building the library and unit tests
- **MinGW Make** — used as the build generator on Windows

### 5.2 Unit Testing
- **GoogleTest** — provided as a submodule and added through CMake
- **CTest** — integrated with the CMake testing framework

### 5.3 Static Analysis
- **cppcheck** — used for static code analysis
- **GCC -fanalyzer** — used to detect potential runtime issues

### 5.4 Code Coverage
- **gcov** — collects raw coverage data
- **gcovr** — generates HTML and text coverage reports

### 5.5 Optional Development Tools
- **PowerShell** — required to run provided `.ps1` scripts
- **Git** — required for cloning submodules

---

## 6. Build Instructions (MinGW + CMake)

### 6.1 Configure and build

```powershell
Remove-Item -Recurse -Force .\build\
cmake -B build -G "MinGW Makefiles" -DBUILD_TESTS=ON -DCOVERAGE=ON .
cmake --build build
```

### 6.2 Running Tests

```powershell
cd build
ctest --verbose
```

## 7. Static Analysis

A script is provided to run static analysis:

Tools:
- cppcheck
- GCC `-fanalyzer`

Execute:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\run_analysis.ps1
```

## 8. Code Coverage

Coverage is generated with **gcov** and **gcovr**, with GoogleTest excluded from reports.

Run:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\run_coverage.ps1
```

Generated reports:

- `coverage_report.txt`
- `coverage_report.html`

## 9. Project Purpose (Student Project)

This implementation was developed as part of a software engineering learning project. The goals included:

- Understanding AUTOSAR Adaptive Platform architecture
- Implementing a real subsystem: State Management
- Developing according to SWE.3 / SWE.4 maturity practices
- Building a full development toolchain:
  - modular implementation
  - unit testing
  - static analysis
  - coverage reporting

## 10. AUTOSAR Specification Reference

This project references:

- **AUTOSAR Adaptive Platform R21-11**
  (Selected requirements from *SWS State Management*)

