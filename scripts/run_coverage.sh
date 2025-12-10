#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD="${ROOT}/build"
REPORT="${ROOT}/coverage_report.txt"
HTML_DIR="${ROOT}/coverage_html"

# Rebuild with coverage enabled
mkdir -p "${BUILD}"
cd "${BUILD}"
cmake -DCOVERAGE=ON -DBUILD_TESTS=ON ..
make -j$(nproc)

# Run unit tests
ctest --output-on-failure

# Collect coverage via lcov
lcov --capture --directory . --output-file coverage.info
# remove system and test framework files
lcov --remove coverage.info '/usr/*' '*/gtest/*' --output-file coverage_filtered.info

# Generate text summary and HTML
genhtml coverage_filtered.info --output-directory "${HTML_DIR}"

# Make a brief text summary
echo "Coverage summary (text) - lines with %:" > "${REPORT}"
lcov --summary coverage_filtered.info 2>&1 | tee -a "${REPORT}"

echo "Coverage HTML in ${HTML_DIR}"
echo "Coverage text report written to ${REPORT}"
