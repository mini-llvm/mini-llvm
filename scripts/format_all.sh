#!/bin/bash

set -e

git ls-files --cached --others --exclude-standard |
  grep -v '^third_party/' |
  grep -E '\.cpp$|\.h$|\.cmake$|(^|/)CMakeLists\.txt$' |
  xargs "$(dirname -- "$0")/format.py" "$@"
