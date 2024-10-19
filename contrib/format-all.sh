#!/bin/sh

script_dir="$(cd -- "$(dirname -- "$0")" && pwd -P)"
"$script_dir/glob.sh" | xargs "$script_dir/format.py" "$@"
