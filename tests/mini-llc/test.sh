#!/bin/bash

set -eu

tests=()

while (( $# > 0 )); do
    case "$1" in
    --help)
        echo "Usage: $0 --mini-llc=<command> --target=<target> [--driver=<command>] [--emulator=<command>] [--output-dir=<dir>] [--timeout=<duration>] <tests>..."
        exit 0
        ;;
    --mini-llc)
        shift
        if (( $# == 0 )); then
            echo "$0: error: missing value for --mini-llc" >&2
            exit 1
        fi
        mini_llc="$1"
        shift
        ;;
    --mini-llc=*)
        mini_llc="${1#*=}"
        shift
        ;;
    --target)
        shift
        if (( $# == 0 )); then
            echo "$0: error: missing value for --target" >&2
            exit 1
        fi
        target="$1"
        shift
        ;;
    --target=*)
        target="${1#*=}"
        shift
        ;;
    --driver)
        shift
        if (( $# == 0 )); then
            echo "$0: error: missing value for --driver" >&2
            exit 1
        fi
        driver="$1"
        shift
        ;;
    --driver=*)
        driver="${1#*=}"
        shift
        ;;
    --emulator)
        shift
        if (( $# == 0 )); then
            echo "$0: error: missing value for --emulator" >&2
            exit 1
        fi
        emulator="$1"
        shift
        ;;
    --emulator=*)
        emulator="${1#*=}"
        shift
        ;;
    --output-dir)
        shift
        if (( $# == 0 )); then
            echo "$0: error: missing value for --output-dir" >&2
            exit 1
        fi
        output_dir="$1"
        shift
        ;;
    --output-dir=*)
        output_dir="${1#*=}"
        shift
        ;;
    --timeout)
        shift
        if (( $# == 0 )); then
            echo "$0: error: missing value for --timeout" >&2
            exit 1
        fi
        timeout="$1"
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
        tests+=("$1")
        shift
        ;;
    esac
done

if [[ ! -v mini_llc ]]; then
    echo "$0: error: missing --mini-llc" >&2
    exit 1
fi

if [[ ! -v target ]]; then
    echo "$0: error: missing --target" >&2
    exit 1
fi

if [[ ! -v driver ]]; then
    driver="$target-linux-gnu-gcc"
fi

if [[ ! -v emulator ]]; then
    emulator="qemu-$target"
fi

if [[ ! -v output_dir ]]; then
    output_dir="$(dirname -- "$0")/results_$(date +%Y%m%d_%H%M%S)"
fi

if [[ ! -v timeout ]]; then
    timeout=60
fi

width=0
for test in "${tests[@]}"; do
    if (( ${#test} > width )); then
        width=${#test}
    fi
done

for test in "${tests[@]}"; do
    printf "%-*s " "$width" "$test"

    if timeout --foreground -v "$timeout" "$(dirname -- "$0")/test_impl.sh" "$test" "$mini_llc" "$target" "$driver" "$emulator" "$output_dir"; then
        echo -e "\033[32mPASSED\033[0m"
    else
        echo -e "\033[31mFAILED\033[0m"
    fi
done
