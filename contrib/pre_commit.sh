#!/bin/bash

set -e

if ! ./contrib/format_staged.sh --check; then
    echo "pre-commit: aborting commit"
    exit 1
fi
