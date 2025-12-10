# =====================================================================
# run_coverage_gcov.ps1 — FINAL WORKING VERSION
# =====================================================================

# REAL project root (folder nadrzędny względem /scripts)
$projectRoot = Split-Path -Parent $PSScriptRoot
Write-Host "Project root: $projectRoot"

$buildDir = "$projectRoot/build"
Write-Host "Build dir:    $buildDir"

# =====================================================================
# Clean old coverage files
# =====================================================================
Write-Host "=== CLEAN OLD COVERAGE FILES ==="
Get-ChildItem -Recurse -Include *.gcda, *.gcno | Remove-Item -Force -ErrorAction SilentlyContinue

# =====================================================================
# Configure with coverage flags
# =====================================================================
Write-Host "=== CONFIGURE PROJECT WITH COVERAGE FLAGS ==="
cmake -B "$buildDir" -G "MinGW Makefiles" -DCOVERAGE=ON "$projectRoot"

# =====================================================================
# Build
# =====================================================================
Write-Host "=== BUILD PROJECT ==="
cmake --build "$buildDir" --target unit_tests

# =====================================================================
# Run tests
# =====================================================================
Write-Host "=== RUN TESTS (generating gcda/gcno files) ==="
& "$buildDir/tests/unit/unit_tests.exe"

# =====================================================================
# RUN GCOV PROPERLY (inside build directory!)
# =====================================================================
Write-Host "=== RUN GCOV (NO googletest) ==="

# katalog, gdzie są GCNO/GCDA
$araSmBuild = "$buildDir/CMakeFiles/ara_sm.dir"

Set-Location $araSmBuild   # <<< KLUCZOWE !!! <<<

# wszystkie gcno z katalogów src i config
$gcnoFiles = Get-ChildItem -Recurse -Filter *.gcno

foreach ($file in $gcnoFiles) {
    Write-Host "GCOV: $($file.FullName)"
    gcov $file.FullName | Out-Null
}

# wracamy do root projektu
Set-Location $projectRoot

# =====================================================================
# Create coverage_report.txt from all .gcov files
# =====================================================================
Write-Host "=== GENERATE coverage_report.txt ==="

# Zbierz wszystkie .gcov z build directory
$gcovFiles = Get-ChildItem -Recurse "$buildDir" -Filter *.gcov

# Zapisz do raportu
$gcovFiles | ForEach-Object {
    "==============================="
    $_.FullName
    "-------------------------------"
    Get-Content $_.FullName
} | Out-File "$projectRoot/coverage_report.txt"

Write-Host "Coverage report saved as: $projectRoot/coverage_report.txt"
Write-Host "=== DONE ==="
