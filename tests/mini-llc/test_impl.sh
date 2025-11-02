#!/bin/bash

set -eu

test="$1"
mini_llc="$2"
target="$3"
driver="$4"
emulator="$5"
output_dir="$6"

mkdir -p "$output_dir"
source "$(dirname -- "$0")/$test.cfg"
$mini_llc --target="$target" -o "$output_dir/$test.s" "$(dirname -- "$0")/$test.ll"
$driver -o "$output_dir/$test" "$output_dir/$test.s" $libs
$emulator "$output_dir/$test" > "$output_dir/$test.out"
diff "$(dirname -- "$0")/$test.ans" "$output_dir/$test.out" >&2
