#!/bin/bash

set -e

ln -sf ../../scripts/pre_commit.sh "$(dirname -- "$0")/../.git/hooks/pre-commit"
ln -sf ../../scripts/pre_push.sh "$(dirname -- "$0")/../.git/hooks/pre-push"
