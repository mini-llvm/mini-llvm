#!/bin/bash

set -e

current_branch="$(git rev-parse --abbrev-ref HEAD)"

if [[ "$current_branch" == "main" ]]; then
    if ! git merge-base --is-ancestor main origin/develop; then
        echo "pre-push: 'main' cannot be ahead of or diverge from 'origin/develop'" >&2
        echo "pre-push: aborting push"
        exit 1
    fi
fi
