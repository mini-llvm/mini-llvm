#!/bin/bash

set -eu

function run_test {
    local test_name="$1"
    local temp_dir="$2"

    local TARGET="${TARGET:-riscv64}"
    local MINI_LLC_COMMAND="${MINI_LLC_COMMAND:-mini-llc}"
    local GCC_COMMAND="${GCC_COMMAND:-riscv64-linux-gnu-gcc}"
    local QEMU_COMMAND="${QEMU_COMMAND:-qemu-riscv64}"
    local DIFF_COMMAND="${DIFF_COMMAND:-diff}"
    local MINI_LLC_TIMEOUT="${MINI_LLC_TIMEOUT:-10}"
    local QEMU_TIMEOUT="${QEMU_TIMEOUT:-10}"

    mkdir -p "$(dirname "$temp_dir/$test_name")" &&
    timeout -v "$MINI_LLC_TIMEOUT" $MINI_LLC_COMMAND --target="$TARGET" -o "$temp_dir/$test_name.s" "$test_name.ll" &&
    $GCC_COMMAND -o "$temp_dir/$test_name" "$temp_dir/$test_name.s" -lm &&
    timeout -v "$QEMU_TIMEOUT" $QEMU_COMMAND "$temp_dir/$test_name" > "$temp_dir/$test_name.out" &&
    $DIFF_COMMAND "${test_name%+*}.ans" "$temp_dir/$test_name.out"
}

function main {
    local temp_dir
    temp_dir="$(mktemp -d)"
    trap 'rm -rf "$temp_dir"' RETURN

    local tests=("$@")
    local failed_tests=()

    for test_name in "${tests[@]}"; do
        printf "%-40s" "$test_name"

        if run_test "$test_name" "$temp_dir"; then
            echo "Passed"
        else
            echo "Failed"
            failed_tests+=("$test_name")
        fi
    done

    if (( "${#failed_tests[@]}" > 0 )); then
        for test_name in "${failed_tests[@]}"; do
            echo "$test_name"
        done
        return 1
    fi
}

main "$@"
