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
# Usage: test-rules.sh [<parser-binary-path>] [<test-data-dir>]
#

PARSER=${1:-"$builddir/usbguard-rule-parser"}
DATADIR=${2:-"$srcdir/src/Tests/Rules/"}

RETVAL=0
IFS=' 	
' # Explicit default IFS (SP HT LN)

echo
echo "Parsing GOOD rules:"
echo "###################"
echo
while read -r rule; do
  if ! $PARSER "$rule"; then
    echo "============="
    echo "FAILED: $rule"
    echo "^^^^^^^^^^^^^"
    RETVAL=1
  fi
done <"$DATADIR/test-rules.good"

echo
echo "Parsing BAD rules:"
echo "##################"
echo
while read -r rule; do
  if $PARSER "$rule"; then
    echo "============="
    echo "FAILED: $rule"
    echo "^^^^^^^^^^^^^"
    RETVAL=1
  fi
done <"$DATADIR/test-rules.bad"

echo
echo "Parsing rules file:"
echo "###################"
echo
$PARSER -f "$DATADIR/test-rules.file" | (
  file_rules=0
  while read -r type rule; do
    if [ "$type" = OUTPUT: ] && [ -n "$rule" ]; then
      echo "FOUND: $rule"
      file_rules=$((file_rules + 1))
    fi
  done

  if [ "$file_rules" -eq 0 ]; then
    echo "======================"
    echo "FAILED: no rules found"
    echo "^^^^^^^^^^^^^^^^^^^^^^"
    exit 1
  fi
) || RETVAL=1

exit $RETVAL
