#!/bin/bash

set -e

git ls-files --cached --others --exclude-standard | grep -v '^third_party/' | grep -E '\.c$|\.cpp$|\.h$' | xargs "$(dirname "$0")/format.py" "$@"
