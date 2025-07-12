#!/bin/bash

set -e

git diff --cached --name-only --diff-filter=ACM | grep -v '^third_party/' | grep -E '\.c$|\.cpp$|\.h$' | xargs "$(dirname "$0")/format.py" "$@"
