#!/bin/bash
#
# check-path-include: src/Library/public/*.hpp
#
SOURCE_FILEPATH="$1"

if grep -q '#[[:space:]]*include[[:space:]]*[<"][[:space:]]*build-config.h[[:space:]]*[">]' "$SOURCE_FILEPATH"; then
  echo
  echo "The public library header files should NOT depend on build-config.h."
  echo "Remove the #include <build-config.h> directive."
  echo
  exit 1
fi

exit 0
# vim: set ts=2 sw=2 et
