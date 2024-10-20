#!/bin/bash

set -eo pipefail

git ls-files --cached --others --exclude-standard | while read -r file; do
    if [[ "$(find "$file" \( -name '*.cpp' -o -name '*.h' \) -not -path 'third_party/*')" ]]; then
        echo "$file"
    fi
done
