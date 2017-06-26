#!/bin/bash
#
# check-path-include: *.cpp
# check-path-include: *.hpp
# check-path-include: *.c
# check-path-include: *.h
#
SOURCE_FILEPATH="$1"
grep -q ' Copyright ' "$SOURCE_FILEPATH"
