#!/usr/bin/env bash

set -euo pipefail

git ls-files --cached --others --exclude-standard |
  { grep -v '^third_party/' || true; } |
  { grep -E '\.cpp$|\.h$|\.cmake$|(^|/)CMakeLists\.txt$|\.bzl$|(^|/)(BUILD|WORKSPACE|MODULE)(\.bazel)?$' || true; } |
  xargs -r "$(dirname -- "$0")/format.py" "$@"
