#!/bin/bash

set -e

ln -s ../../contrib/pre_commit.sh .git/hooks/pre-commit
ln -s ../../contrib/pre_push.sh .git/hooks/pre-push
