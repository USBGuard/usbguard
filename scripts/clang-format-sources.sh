#!/bin/bash
#
# Copyright (C) 2026 Attila Lakatos
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Authors: Attila Lakatos <alakatos@redhat.com>
#
# Unified clang-format helper for usbguard sources.
#
# Usage:
#   clang-format-sources.sh --check [FILE...]
#       Check formatting; print a diff of required changes to stdout.
#       Exits 0 if all files are correctly formatted, 1 if any need
#       reformatting, or 2 if clang-format is not available or too old.
#
#   clang-format-sources.sh --fix [FILE...]
#       Reformat sources in-place.
#       Exits 0 on success, 2 if clang-format is not available or too old.
#
# If no FILE arguments are given, operates on all non-generated,
# non-ThirdParty C/C++ sources under src/.

set -e -o pipefail

PROJECT_ROOT="${PROJECT_ROOT:-$(cd "$(dirname "$0")/.." && pwd)}"

CLANG_FORMAT="$(command -v clang-format-18 \
             || command -v clang-format-17 \
             || command -v clang-format-16 \
             || command -v clang-format \
             || true)"

if [[ -z "${CLANG_FORMAT}" ]]; then
  echo "ERROR: clang-format not found. Install clang-format >= 15." >&2
  exit 2
fi

CF_MAJOR=$(${CLANG_FORMAT} --version \
           | sed -n 's/.*clang-format version \([0-9]\+\).*/\1/p')
if [[ -z "${CF_MAJOR}" || "${CF_MAJOR}" -lt 15 ]]; then
  echo "ERROR: clang-format ${CF_MAJOR} is too old (need >= 15)." >&2
  exit 2
fi

case "${1:-}" in
  --check) MODE="check" ;;
  --fix)   MODE="fix"   ;;
  *)
    echo "Usage: $(basename "$0") --check|--fix [FILE...]" >&2
    exit 1
    ;;
esac
shift

if [[ $# -gt 0 ]]; then
  FILES=("$@")
else
  mapfile -t FILES < <(
    find "${PROJECT_ROOT}/src" \
      -not -path '*ThirdParty/*' \
      -not \( -name '*.pb.h' -o -name 'build-config.h' \) \
      \( -name '*.cpp' -o -name '*.hpp' -o -name '*.c' -o -name '*.h' \) \
      | sort
  )
fi

case "${MODE}" in
  check)
    needs_fix=0
    tmpfile=$(mktemp)
    trap 'rm -f "${tmpfile}"' EXIT
    for f in "${FILES[@]}"; do
      "${CLANG_FORMAT}" "${f}" > "${tmpfile}"
      if ! diff -q "${f}" "${tmpfile}" > /dev/null 2>&1; then
        diff -u --color=always "${f}" "${tmpfile}" || true
        needs_fix=1
      fi
    done
    exit ${needs_fix}
    ;;
  fix)
    "${CLANG_FORMAT}" -i "${FILES[@]}"
    ;;
esac
