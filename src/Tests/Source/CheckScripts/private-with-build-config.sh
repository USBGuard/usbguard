#!/bin/bash
#
# check-path-include: *.cpp
# check-path-include: *.hpp
# check-path-include: *.c
# check-path-include: *.h
# check-path-exclude: src/Library/public/*.hpp
# check-path-exclude: src/Tests/*
# check-path-exclude: src/test_filesystem.cpp
#
SOURCE_FILEPATH="$1"

FIRST_IFDEF="$(sed -r -n 's/^#[[:space:]]*(ifdef|if|include|define)/&/p' "$SOURCE_FILEPATH" | head -1)"

if [[ "$FIRST_IFDEF" != "#ifdef HAVE_BUILD_CONFIG_H" ]]; then
  echo
  echo "All source files should include build-config.h before any other file, add: "
  echo "--- snip ---"
  echo " #ifdef HAVE_BUILD_CONFIG_H"
  echo "   #include <build-config.h>"
  echo " #endif"
  echo "--- snip ---"
  echo "to the beginning of the source file."
  echo
  exit 1
fi

exit 0
# vim: set ts=2 sw=2 et
