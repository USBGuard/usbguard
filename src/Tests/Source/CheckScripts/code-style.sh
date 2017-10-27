#!/bin/bash
#
# check-path-include: *.cpp
# check-path-include: *.hpp
# check-path-include: *.c
# check-path-include: *.h
#
set -e -o pipefail

SOURCE_FILEPATH="$1"
ASTYLE="${PROJECT_ROOT}/scripts/astyle.sh"
ASTYLERC_PATH="${PROJECT_ROOT}/src/astylerc"
ASTYLE_VERSION_MAJOR=$(${ASTYLE} -V | sed -n 's|^Artistic Style Version \([0-9]\+\)\.[0-9]\+\.[0-9]\+|\1|p')

if [[ "$ASTYLE_VERSION_MAJOR" -lt 3 ]]; then
  exit 77
fi

if [[ -n "$(${ASTYLE} --formatted --dry-run $(< ${ASTYLERC_PATH}) "$SOURCE_FILEPATH")" ]]; then
   exit 1
fi

exit 0
