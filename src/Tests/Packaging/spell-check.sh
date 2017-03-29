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
##################

ASPELL=$(which aspell)
PANDOC=$(which pandoc)

if [[ -z "$ASPELL" || -z "$PANDOC" ]]; then
  echo "Skipping test because some dependecies are missing"
  echo "ASPELL: $ASPELL"
  echo "PANDOC: $PANDOC"
  exit 77
fi

EXTRA_DICTIONARY="./${srcdir}/src/Tests/Packaging/spell-check.rws"
ASPELL_OPTIONS="-W 2 --add-extra-dicts=$EXTRA_DICTIONARY --add-html-skip=code"

PATH_PATTERN_INCLUDE=(
${srcdir}/README.md
${srcdir}/doc/*.md
)

##################
retval=0
set -o pipefail
export LANG="en_US.UTF-8"
while read path; do
  WORDS="$($PANDOC -t html $path | $ASPELL -l en --add-filter=html $ASPELL_OPTIONS list | sort | uniq -c)"
  if [[ -n "$WORDS" ]]; then
    echo
    echo "ERROR: Found missspelled words in $(readlink -f "$path"):"
    echo "========================================"
    echo -e "$WORDS"
    echo "========================================"
    echo "To whitelist a word, add it to this dictionary:"
    echo "  $(readlink -f "$EXTRA_DICTIONARY")"
    echo
    retval=1
  fi
done <<EOF
$(ls -1 ${PATH_PATTERN_INCLUDE[*]})
EOF
exit $retval

