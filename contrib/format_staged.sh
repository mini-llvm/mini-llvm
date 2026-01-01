#!/bin/bash

set -e

git diff --cached --name-only --diff-filter=ACMR |
  grep -v '^third_party/' |
  grep -E '\.c$|\.cpp$|\.h$|\.cmake$|(^|/)CMakeLists\.txt$' |
  xargs "$(dirname -- "$0")/format.py" "$@"
