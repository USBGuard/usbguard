#!/bin/bash
#
# Copyright (C) 2016 Red Hat, Inc.
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
# Authors: Daniel Kopecek <dkopecek@redhat.com>
#
#
# This script looks for files that don't contain copyright infomation.
# It does so very naively by searching for files that do not contain
# "Copyright " in their contents.
#
##################
#set -x

GLOB_INCLUDE=(
*.[ch]pp
*.[ch]
*.am
*.qrc
*.sh
*.md
*.adoc
)

GLOB_EXCLUDE=(
*ThirdParty/*
*build/*
*m4/*
*.pb.*
*build-config.h
)

##################

function __realpath () {
  path="$1"
  base="$2"
  abs_path=$(readlink -f "$path")

  if [[ -n "$2" ]]; then
    abs_base="$(readlink -f "$base")"
    abs_path="$(echo "$abs_path" | sed "s|^${abs_base}/\?||")"
  fi

  echo "$abs_path"
}

if [[ -z "${srcdir}" ]]; then
  echo "srcdir variable not set!"
  exit 1
fi

PROJECT_ROOT="${abs_top_srcdir}"
SOURCE_ROOT="${srcdir}"
CHECK_SCRIPTS_DIR="${srcdir}/src/Tests/Source/CheckScripts/"

for glob in ${GLOB_INCLUDE[*]}; do
  INCLUDE_ARGS+=" -o -path $glob"
done

for glob in ${GLOB_EXCLUDE[*]}; do
  EXCLUDE_ARGS+=" -o -path $glob"
done

FIND_ARGS="( -false $INCLUDE_ARGS ) -not ( -false $EXCLUDE_ARGS )"

driver_retval=0

while read source_filepath; do
  source_relpath="$(__realpath "${source_filepath}" "${SOURCE_ROOT}")"
  failed_checks_output="File: ${source_relpath}\n"
#  debug_checks_output="File: ${source_relpath}\n"
  some_checks_failed=0

  while read script_filepath; do
    path_include_match=0
    path_include_found=0
    path_exclude_match=0
    path_exclude_found=0
    #
    # Process check-path-exclude:
    #
    while read check_path_exclude; do
      path_exclude_found=1
      if [[ $source_relpath = $check_path_exclude ]]; then
        path_exclude_match=1
        break
      fi
    done<<EOF
$(sed -n 's|.*check-path-exclude: \(.*\)$|\1|p' "${script_filepath}")
EOF

    if [[ $path_exclude_match -eq 1 ]]; then
      # Skip executing the check
      continue
    fi

    #
    # Process check-path-include:
    #
    while read check_path_include; do
      path_include_found=1
      if [[ $source_relpath = $check_path_include ]]; then
        path_include_match=1
        break
      fi
    done<<EOF
$(sed -n 's|.*check-path-include: \(.*\)$|\1|p' "${script_filepath}")
EOF

    if [[ $path_include_match -eq 0 ]]; then
      # Skip executing the check
      continue
    fi

    #
    #
    # Run check script
    #
    env PROJECT_ROOT="${PROJECT_ROOT}" SOURCE_ROOT="$SOURCE_ROOT" "${script_filepath}" "${source_filepath}"

    #
    # Process return value
    #
    retval=$?
    check_failed=0

    if [[ $retval -ne 0 ]]; then
      if [[ $retval -eq 77 ]]; then
        check_status="skipped"
      else
        check_status="FAILED!"
        check_failed=1
      fi
    else
      check_status="success"
    fi

    if [[ $check_failed -ne 0 ]]; then
      failed_checks_output="${failed_checks_output}$(printf '%28s: %s' "$(basename "${script_filepath}")" "$check_status")\n"
      some_checks_failed=1
#    else
#      debug_checks_output="${debug_checks_output}$(printf '%28s: %s' "$(basename "${script_filepath}")" "$check_status")\n"
    fi

  done <<EOF
$(find "${CHECK_SCRIPTS_DIR}" -type f -executable -not -name '*~')
EOF

if [[ $some_checks_failed -eq 1 ]]; then
  echo -e "${failed_checks_output}"
  driver_retval=1
fi

#echo -e "${debug_checks_output}"

done <<EOF
$(find "${SOURCE_ROOT}" ${FIND_ARGS})
EOF

exit $driver_retval
