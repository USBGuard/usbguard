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
# Usage: test-rules.sh <parser-binary-path> <test-data-dir>
#        test-rules.sh
#

if [ -z "$1" -o -z "$2" ]; then
  PARSER="$builddir/usbguard-rule-parser"
  DATADIR="$srcdir/src/Tests/Rules/"
else
  USBGUARD="$1"
  DATADIR="$2"
fi

#TEMPDIR="$(mktemp -d --tmpdir usbguard-test-descriptor-parser.XXXXXX)"
RETVAL=0
export IFS=

echo
echo "Parsing GOOD rules:"
echo "###################"
echo
while read -r rule; do
  $PARSER "$rule"
  if [ $? -ne 0 ]; then
    echo "============="
    echo "FAILED: $rule"
    echo "^^^^^^^^^^^^^"
    RETVAL=1
  fi
done <<EOF
$(cat $DATADIR/test-rules.good)
EOF

echo
echo "Parsing BAD rules:"
echo "##################"
echo
while read -r rule; do
  $PARSER "$rule"
  if [ $? -eq 0 ]; then
    echo "============="
    echo "FAILED: $rule"
    echo "^^^^^^^^^^^^^"
    RETVAL=1
  fi
done <<EOF
$(cat $DATADIR/test-rules.bad)
EOF

exit $RETVAL
