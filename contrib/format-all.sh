#!/bin/sh

script_dir="$(cd -- "$(dirname -- "$0")" && pwd -P)"
find . \( -name '*.cpp' -o -name '*.h' \) -not -path './third_party/*' | xargs "$script_dir/format.py" "$@"
