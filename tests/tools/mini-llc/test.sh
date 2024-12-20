#!/bin/bash

set -eu

function run_test {
    local test_name="$1"
    local target="$2"
    local result_dir="$3"

    mkdir -p "$(dirname "$result_dir/$test_name")" &&
    timeout -v "$MINI_LLC_TIMEOUT" $MINI_LLC_COMMAND --target="$target" -o "$result_dir/$test_name.s" "$test_name.ll" &&
    timeout -v "$LINKER_TIMEOUT" $LINKER_COMMAND -o "$result_dir/$test_name" "$result_dir/$test_name.s" -lm &&
    timeout -v "$EMULATOR_TIMEOUT" $EMULATOR_COMMAND "$result_dir/$test_name" > "$result_dir/$test_name.out" &&
    $DIFF_COMMAND "${test_name}.ans" "$result_dir/$test_name.out"
}

function main {
    local target=""
    local result_dir=""
    local tests=()

    while (( $# > 0 )); do
        case "$1" in
        --help)
            echo "Usage: $0 --target=<target> [--result-dir=<result-dir>] <tests>..."
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
            tests+=("$1")
            shift
            ;;
        esac
    done

    if [[ -z "$target" ]]; then
        echo "$0: error: no target" >&2
        exit 1
    fi

    if [[ -z "$result_dir" ]]; then
        result_dir="result_$(date +%Y%m%d_%H%M%S)"
    fi

    mkdir -p "$result_dir"
    echo "*" > "$result_dir/.gitignore"

    MINI_LLC_COMMAND="${MINI_LLC_COMMAND:-mini-llc}"
    LINKER_COMMAND="${LINKER_COMMAND:-riscv64-linux-gnu-gcc}"
    EMULATOR_COMMAND="${EMULATOR_COMMAND:-qemu-riscv64}"
    DIFF_COMMAND="${DIFF_COMMAND:-diff}"
    MINI_LLC_TIMEOUT="${MINI_LLC_TIMEOUT:-10}"
    LINKER_TIMEOUT="${LINKER_TIMEOUT:-10}"
    EMULATOR_TIMEOUT="${EMULATOR_TIMEOUT:-10}"

    local width=0
    for test_name in "${tests[@]}"; do
        if (( ${#test_name} > width )); then
            width=${#test_name}
        fi
    done

    local failed_tests=()

    for test_name in "${tests[@]}"; do
        printf "%-*s " "$width" "$test_name"

        if run_test "$test_name" "$target" "$result_dir"; then
            echo "Passed"
        else
            echo "Failed"
            failed_tests+=("$test_name")
        fi
    done

    if (( "${#failed_tests[@]}" > 0 )); then
        for test_name in "${failed_tests[@]}"; do
            echo "$test_name" >> "$result_dir/failed_tests.txt"
        done
        return 1
    fi
}

main "$@"
