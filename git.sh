#!/bin/sh

git add -A
git status
git commit -m "$1"
git push
