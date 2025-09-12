#!/bin/bash

set -e

git ls-files --cached --others --exclude-standard |
  grep -v '^third_party/' |
  grep -E '\.c$|\.cpp$|\.h$' |
  xargs "$(dirname -- "$0")/format.py" "$@"

git ls-files --cached --others --exclude-standard |
  grep -v '^third_party/' |
  grep -E '(^|/)CMakeLists\.txt$|\.cmake$' |
  xargs "$(dirname -- "$0")/cmake_format.py" "$@"
