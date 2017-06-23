#!/bin/bash
#
# check-path-include: *.cpp
# check-path-include: *.hpp
# check-path-exclude: src/ThirdParty/*
#
SOURCE_FILEPATH="$1"
VIM_MODELINE="$(< "$SOURCE_ROOT"/scripts/modeline.vim)"
exit 0
grep -q " vim: $VIM_MODELINE" "$SOURCE_FILEPATH"
