#!/bin/sh
set -x -e
git submodule update --init
for reconf_dir in "./" "./src/ThirdParty/libqb" "./src/ThirdParty/libsodium"; do
  mkdir -p "$reconf_dir/m4"
  autoreconf -i -s --no-recursive "$reconf_dir"
done
