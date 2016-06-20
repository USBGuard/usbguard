#!/bin/sh
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
# Tests the descriptor parser by parsing binary descriptor samples
# and comparing the output with the expected output (files with .out
# extension)
#
# Usage: test-descriptor-parser.sh <usbguard-binary-path> <test-data-dir>
#        test-descriptor-parser.sh
#

if [ -z "$1" -o -z "$2" ]; then
  USBGUARD="$builddir/usbguard"
  DATADIR="$srcdir/src/Tests/USB/data/"
else
  USBGUARD="$1"
  DATADIR="$2"
fi

TEMPDIR="$(mktemp -d --tmpdir usbguard-test-descriptor-parser.XXXXXX)"
RETVAL=0

for sample in $DATADIR/*.bin; do
  OUTPUT_PATH="$TEMPDIR/$(basename -s .bin $sample).out"
  BINOUT_PATH="$DATADIR/$(basename -s .bin $sample).out"
  $USBGUARD read-descriptor "$sample" > "$OUTPUT_PATH"
  diff -u "$BINOUT_PATH" "$OUTPUT_PATH"
  if [ $? -ne 0 ]; then
    echo "FAILED: $sample"
    RETVAL=1
  fi
done

if [ $RETVAL -eq 0 ]; then
  rm "$TEMPDIR"/*.out
  rmdir "$TEMPDIR"
fi

exit $RETVAL
