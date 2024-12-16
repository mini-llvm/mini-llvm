#!/bin/bash

set -eu

if [[ "$#" -eq 1 && "$1" == "--help" ]]; then
    echo "Usage: $0 --target=<target> [--result-dir=<result-dir>]"
    exit 0
fi

script_dir="$(cd -- "$(dirname -- "$0")" && pwd -P)"
"$script_dir/tests.sh" | xargs "$script_dir/test.sh" "$@"
