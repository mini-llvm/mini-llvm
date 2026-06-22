#!/usr/bin/env bash

set -euo pipefail

git ls-files --cached --others --exclude-standard \
    ':(glob)**/*.cpp' \
    ':(glob)**/*.h' \
    ':(glob)**/*.cmake' \
    ':(glob)**/CMakeLists.txt' \
    ':(glob)**/*.bzl' \
    ':(glob)**/BUILD' \
    ':(glob)**/BUILD.bazel' \
    ':(glob)**/WORKSPACE' \
    ':(glob)**/WORKSPACE.bazel' \
    ':(glob)**/MODULE.bazel' \
    ':(exclude)third_party/' \
  | xargs -r "$(dirname -- "$0")/format.py" "$@"
