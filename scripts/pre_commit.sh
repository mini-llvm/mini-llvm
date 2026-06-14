#!/usr/bin/env bash

set -euo pipefail

if ! ./scripts/format_staged.sh --check; then
  echo "pre-commit: aborting commit"
  exit 1
fi
