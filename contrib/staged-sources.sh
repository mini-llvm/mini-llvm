#!/bin/sh

git diff --cached --name-only | grep -E '\.cpp$|\.h$' | grep -v '^third_party/'
