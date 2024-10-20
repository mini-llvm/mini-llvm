#!/bin/sh

git ls-files --cached --others --exclude-standard | grep -E '\.cpp$|\.h$' | grep -v '^third_party/'
