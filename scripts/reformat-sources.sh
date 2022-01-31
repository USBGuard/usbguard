#!/bin/bash
#
# Copyright (C) 2017 Red Hat, Inc.
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
PROJECT_ROOT="$(dirname "$0")/../"
ASTYLE="${PROJECT_ROOT}/scripts/astyle.sh"

echo
echo " PLEASE READ:"
echo " ============"
echo " This script requires AStyle version 3 and above."
echo " Older versions won't work correctly (or not at all)."
echo " ============"
echo

set -ex

find "${PROJECT_ROOT}/src" \
	-type f -not -path '*ThirdParty/*' \
	-not \( -name \*.pb.h -o -name build-config.h \) \
	\( -name '*.cpp' -or -name '*.hpp' -or -name '*.c' -or -name '*.h' \) \
	-exec "${ASTYLE}" --preserve-date --suffix=none --lineend=linux --formatted "{}" \;
