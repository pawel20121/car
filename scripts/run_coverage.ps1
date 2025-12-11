# Wymuś generator MinGW Makefiles
$generator = "MinGW Makefiles"
$buildDir = "build_coverage"

$env:PATH = "C:\msys64\mingw64\bin;$env:PATH"

if (Test-Path $buildDir) {
    Remove-Item $buildDir -Recurse -Force
}

cmake -G $generator -B $buildDir -S . -DCOVERAGE=ON -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build $buildDir --target unit_tests

ctest --test-dir $buildDir --output-on-failure

# TYLKO coverage dla Twojego kodu (bez googletest!)
py -m gcovr -r . --exclude="external" --txt -o coverage_report.txt
py -m gcovr -r . --exclude="external" --html-details -o coverage_report.html

Write-Host "Coverage reports generated: coverage_report.txt, coverage_report.html"
