#!/bin/bash

set -e

if ! ./scripts/format_staged.sh --check; then
    echo "pre-commit: aborting commit"
    exit 1
fi
