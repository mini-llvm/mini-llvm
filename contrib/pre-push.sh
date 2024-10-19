#!/bin/bash

set -e

current_branch="$(git rev-parse --abbrev-ref HEAD)"

if [[ "$current_branch" == "main" ]]; then
    if ! git merge-base --is-ancestor main origin/develop; then
        echo "pre-push: error: 'main' cannot be ahead of or diverge from 'origin/develop'"
        exit 1
    fi
fi
