# =============================
# RUN STATIC ANALYSIS
# =============================

$projectRoot = Split-Path -Parent $PSScriptRoot
Write-Host "Project root: $projectRoot"

$analysisOutput = Join-Path $projectRoot "cppcheck_results.txt"

# -----------------------------
# RUN CPPCHECK (NO googletest)
# -----------------------------
Write-Host "Running Cppcheck..."

cppcheck `
    --enable=all `
    --inconclusive `
    --std=c++17 `
    --force `
    --quiet `
    --suppress=missingIncludeSystem `
    --output-file="$analysisOutput" `
    "$projectRoot/src" `
    "$projectRoot/config"

Write-Host "Cppcheck report: $analysisOutput"


# -----------------------------
# RUN DOXYGEN (NO googletest)
# -----------------------------
Write-Host "Running Doxygen..."

# Create default config if missing
if (!(Test-Path "$projectRoot/Doxyfile")) {
    doxygen -g "$projectRoot/Doxyfile"
}

# Fix INPUT paths – EXCLUDE external/googletest
(Get-Content "$projectRoot/Doxyfile") `
    -replace '^INPUT .*', "INPUT = $projectRoot/src $projectRoot/config $projectRoot/README.md" `
    -replace '^EXCLUDE .*', "EXCLUDE = $projectRoot/external" `
    | Set-Content "$projectRoot/Doxyfile"

doxygen "$projectRoot/Doxyfile"

Write-Host "Doxygen documentation: doc/html/index.html"
Write-Host "=== ANALYSIS DONE ==="
