#!/bin/sh

script_dir="$(cd -- "$(dirname -- "$0")" && pwd -P)"
"$script_dir/tests.sh" | xargs "$script_dir/test.sh" "$@"
