#!/bin/bash

set -eu

test="$1"
mini_llc="$2"
target="$3"
driver="$4"
emulator="$5"
result_dir="$6"

mkdir -p "$result_dir" 
$mini_llc --target="$target" -o "$result_dir/$test.s" "$(dirname "$0")/$test.ll"
$driver -o "$result_dir/$test" "$result_dir/$test.s" -lm
$emulator "$result_dir/$test" > "$result_dir/$test.out"
diff "$(dirname "$0")/$test.ans" "$result_dir/$test.out" >&2
