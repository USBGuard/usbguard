#!/bin/bash
#
# check-path-include: *.cpp
# check-path-include: *.hpp
# check-path-include: *.c
# check-path-include: *.h
#
set -e -o pipefail

SOURCE_FILEPATH="${1:?needs one filename as an argument}"
ASTYLE="${PROJECT_ROOT:?not set in the environment}/scripts/astyle.sh"
ASTYLE_VERSION_MAJOR=$(${ASTYLE} -V | sed -n 's|^Artistic Style Version \([0-9]\+\)\.[0-9]\+\(\.[0-9]\+\)\?|\1|p')

if [[ "$ASTYLE_VERSION_MAJOR" -lt 3 ]]; then
  exit 77
fi

tempfile="$(mktemp)"
delete_tempfile() {
    rm -f "${tempfile}"
}
trap delete_tempfile EXIT

"${ASTYLE}" < "${SOURCE_FILEPATH}" > "${tempfile}"

# NOTE: We cannot use "exec" here or the trap callback above will not be run
# NOTE: This is meant to return code 1 on non-empty diff
diff -u --color=always "${SOURCE_FILEPATH}" "${tempfile}"
