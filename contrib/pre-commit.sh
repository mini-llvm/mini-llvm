#!/bin/bash

set -e

if ! ./contrib/format-all.sh --check; then
    echo "pre-commit: error: code not properly formatted" >&2
    exit 1
fi
