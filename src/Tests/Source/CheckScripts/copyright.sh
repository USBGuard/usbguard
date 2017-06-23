#!/bin/bash
#
# check-path-include: *.cpp
# check-path-include: *.hpp
#
SOURCE_FILEPATH="$1"
grep -q ' Copyright ' "$SOURCE_FILEPATH"
