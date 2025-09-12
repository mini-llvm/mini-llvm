#!/bin/bash

set -e

git diff --cached --name-only --diff-filter=ACMR |
  grep -v '^third_party/' |
  grep -E '\.c$|\.cpp$|\.h$' |
  xargs "$(dirname -- "$0")/format.py" "$@"

git diff --cached --name-only --diff-filter=ACMR |
  grep -v '^third_party/' |
  grep -E '(^|/)CMakeLists\.txt$|\.cmake$' |
  xargs "$(dirname -- "$0")/cmake_format.py" "$@"
