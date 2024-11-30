#!/bin/sh

script_dir="$(cd -- "$(dirname -- "$0")" && pwd -P)"
"$script_dir/sources-staged.sh" | xargs "$script_dir/format.py" "$@"
