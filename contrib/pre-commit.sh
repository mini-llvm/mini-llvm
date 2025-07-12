#!/bin/bash

set -e

if ! ./contrib/format-staged.sh --check; then
    echo "pre-commit: aborting commit"
    exit 1
fi
