param(
    [string]$buildDir = "build"
)

# Katalog projektu = katalog nadrzędny dla scripts/
$projectRoot = Split-Path $PSScriptRoot -Parent

# Pełna ścieżka build/
$buildPath = Join-Path $projectRoot $buildDir
$unitTestsExe = Join-Path $buildPath "tests\unit\unit_tests.exe"

Write-Host "Project root: $projectRoot"
Write-Host "Build dir:    $buildPath"

if (!(Test-Path $buildPath)) {
    Write-Error "Build directory not found: $buildPath"
    exit 1
}

# 1. Build tests
Write-Host "Building unit tests..."
cmake --build $buildPath --target unit_tests

# 2. Run unit tests
if (!(Test-Path $unitTestsExe)) {
    Write-Error "unit_tests.exe not found: $unitTestsExe"
    exit 1
}

Write-Host "Running unit_tests..."
& $unitTestsExe

# 3. Coverage reports
Write-Host "Generating coverage reports..."

gcovr -r $projectRoot --xml-pretty   -o "$projectRoot\coverage_report.xml"
gcovr -r $projectRoot --txt          -o "$projectRoot\coverage_report.txt"
gcovr -r $projectRoot --html-details -o "$projectRoot\coverage_report.html"

Write-Host ""
Write-Host "Coverage reports generated:"
Write-Host "  $projectRoot\coverage_report.txt"
Write-Host "  $projectRoot\coverage_report.html"
