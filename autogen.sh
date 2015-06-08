#!/bin/sh
set -x -e
git submodule update --init
mkdir -p m4
for reconf_dir in "./" "./src/ThirdParty/libqb" "./src/ThirdParty/libsodium"; do
    autoreconf -i -s --no-recursive "$reconf_dir"
done
