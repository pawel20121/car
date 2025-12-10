#!/usr/bin/env bash
set -euo pipefail

# katalog projektu = jeden poziom wyżej względem scripts/
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT="${ROOT}/cppcheck_report.txt"

cppcheck --enable=all --inconclusive --std=posix --language=c++ \
    --suppress=missingIncludeSystem \
    -I "${ROOT}/include" \
    "${ROOT}/src" "${ROOT}/include" 2>&1 | tee "${OUT}"

echo "cppcheck report written to ${OUT}"
