#!/bin/bash

set -e

ln -sf ../../contrib/pre_commit.sh .git/hooks/pre-commit
ln -sf ../../contrib/pre_push.sh .git/hooks/pre-push
