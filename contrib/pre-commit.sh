#!/bin/bash

set -e

if ! ./contrib/format-all.sh --check; then
    exit 1
fi
