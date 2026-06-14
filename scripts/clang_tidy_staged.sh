#!/usr/bin/env bash

set -euo pipefail

git diff --cached --name-only --diff-filter=ACMR -- \
    ':(glob)**/*.cpp' \
    ':(glob)**/*.h' \
    ':(exclude)third_party/' \
  | xargs -r "$(dirname -- "$0")/clang_tidy.py" "$@"
