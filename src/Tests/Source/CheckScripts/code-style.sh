#!/bin/bash
#
# check-path-include: *.cpp
# check-path-include: *.hpp
# check-path-include: *.c
# check-path-include: *.h
#
# Automake adapter for clang-format-sources.sh.
# Translates exit code 2 (clang-format unavailable) into exit code 77 (skip).

SOURCE_FILEPATH="${1:?needs one filename as an argument}"

"${PROJECT_ROOT:?not set in the environment}/scripts/clang-format-sources.sh" \
  --check "${SOURCE_FILEPATH}"
ret=$?

if [[ ${ret} -eq 2 ]]; then
  exit 77
fi
exit ${ret}
