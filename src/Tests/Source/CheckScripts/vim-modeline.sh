#!/bin/bash
#
# check-path-include: *.cpp
# check-path-include: *.hpp
# check-path-include: *.c
# check-path-include: *.h
# check-path-exclude: src/ThirdParty/*
#
SOURCE_FILEPATH="$1"
VIM_MODELINE="$(< "$SOURCE_ROOT"/scripts/modeline.vim)"

grep -q " vim: $VIM_MODELINE" "$SOURCE_FILEPATH"
