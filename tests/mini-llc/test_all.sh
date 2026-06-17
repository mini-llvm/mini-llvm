#!/usr/bin/env bash

set -euo pipefail

while (( $# > 0 )); do
  case "$1" in
  --help)
    echo "Usage: $0 --mini-llc <mini-llc> --target <target> [--driver <driver>] [--emulator <emulator>] [--output-dir <output-dir>] [--timeout <timeout>] [--jobs <jobs>]"
    exit 0
    ;;
  --mini-llc)
    shift
    if (( $# == 0 )); then
      echo "$0: error: missing value for '--mini-llc'" >&2
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
      echo "$0: error: missing value for '--target'" >&2
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
      echo "$0: error: missing value for '--driver'" >&2
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
      echo "$0: error: missing value for '--emulator'" >&2
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
      echo "$0: error: missing value for '--output-dir'" >&2
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
      echo "$0: error: missing value for '--timeout'" >&2
      exit 1
    fi
    timeout="$1"
    shift
    ;;
  --timeout=*)
    timeout="${1#*=}"
    shift
    ;;
  --jobs)
    shift
    if (( $# == 0 )); then
      echo "$0: error: missing value for '--jobs'" >&2
      exit 1
    fi
    jobs="$1"
    shift
    ;;
  --jobs=*)
    jobs="${1#*=}"
    shift
    ;;
  -*)
    echo "$0: error: unrecognized option '$1'" >&2
    exit 1
    ;;
  *)
    echo "$0: error: unrecognized argument '$1'" >&2
    exit 1
    ;;
  esac
done

args=()

[[ -v mini_llc ]] && args+=(--mini-llc "$mini_llc")
[[ -v target ]] && args+=(--target "$target")
[[ -v driver ]] && args+=(--driver "$driver")
[[ -v emulator ]] && args+=(--emulator "$emulator")
[[ -v output_dir ]] && args+=(--output-dir "$output_dir")
[[ -v timeout ]] && args+=(--timeout "$timeout")
[[ -v jobs ]] && args+=(--jobs "$jobs")

for f in "$(dirname -- "$0")"/*.ll; do
  args+=("$(basename "$f" .ll)")
done

"$(dirname -- "$0")/test.sh" "${args[@]}"
