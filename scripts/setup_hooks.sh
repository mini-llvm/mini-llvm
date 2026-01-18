#!/bin/bash

set -e

ln -sf ../../scripts/pre_commit.sh .git/hooks/pre-commit
ln -sf ../../scripts/pre_push.sh .git/hooks/pre-push
