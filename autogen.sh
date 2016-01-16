#!/bin/sh
set -x -e
git submodule update --init
mkdir -p ./m4
autoreconf -i -s --no-recursive ./

