#!/usr/bin/env bash

set -euo pipefail

git ls-files --cached --others --exclude-standard \
    ':(glob)**/*.cpp' \
    ':(glob)**/*.h' \
    ':(exclude)third_party/' \
  | xargs -r "$(dirname -- "$0")/clang_tidy.py" "$@"
