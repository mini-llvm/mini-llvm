#!/bin/bash

set -eu

target=""
result_dir=""

while (( $# > 0 )); do
    case "$1" in
    --help)
        echo "Usage: $0 --target=<target> [--result-dir=<result-dir>]"
        exit 0
        ;;
    --target=*)
        target="${1#*=}"
        shift
        ;;
    --result-dir=*)
        result_dir="${1#*=}"
        shift
        ;;
    -*)
        echo "$0: error: unrecognized option: $1" >&2
        exit 1
        ;;
    *)
        echo "$0: error: unrecognized argument: $1" >&2
        exit 1
        ;;
    esac
done

args=()

if [[ -n "$target" ]]; then
    args+=("--target=$target")
else
    echo "$0: error: no target" >&2
    exit 1
fi
if [[ -n "$result_dir" ]]; then
    args+=("--result-dir=$result_dir")
fi

script_dir="$(cd -- "$(dirname -- "$0")" && pwd -P)"
"$script_dir/tests.sh" | xargs "$script_dir/test.sh" "${args[@]}"
