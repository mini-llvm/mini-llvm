#!/bin/bash

set -e

if ! ./contrib/check-all.sh || ! ./contrib/format-all.sh --check; then
    echo "pre-commit: aborting commit"
    exit 1
fi
