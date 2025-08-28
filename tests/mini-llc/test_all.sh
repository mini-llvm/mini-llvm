#!/bin/bash

set -eu

while (( $# > 0 )); do
    case "$1" in
    --help)
        echo "Usage: $0 --mini-llc=<command> --target=<target> [--driver=<command>] [--emulator=<command>] [--result-dir=<dir>] [--timeout=<duration>]"
        exit 0
        ;;
    --mini-llc=*)
        mini_llc="${1#*=}"
        shift
        ;;
    --target=*)
        target="${1#*=}"
        shift
        ;;
    --driver=*)
        driver="${1#*=}"
        shift
        ;;
    --emulator=*)
        emulator="${1#*=}"
        shift
        ;;
    --result-dir=*)
        result_dir="${1#*=}"
        shift
        ;;
    --timeout=*)
        timeout="${1#*=}"
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

if [[ -v mini_llc ]]; then
    args+=("--mini-llc=$mini_llc")
fi

if [[ -v target ]]; then
    args+=("--target=$target")
fi

if [[ -v driver ]]; then
    args+=("--driver=$driver")
fi

if [[ -v emulator ]]; then
    args+=("--emulator=$emulator")
fi

if [[ -v result_dir ]]; then
    args+=("--result-dir=$result_dir")
fi

if [[ -v timeout ]]; then
    args+=("--timeout=$timeout")
fi

tests=()

for f in "$(dirname "$0")"/*.ll; do
    tests+=("$(basename "$f" .ll)")
done

"$(dirname "$0")/test.sh" "${args[@]}" "${tests[@]}"
