#!/bin/sh

git diff --cached --name-only --diff-filter=ACM | grep -E '\.cpp$|\.h$' | grep -v '^third_party/'
