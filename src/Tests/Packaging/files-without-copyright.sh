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
GLOBS_INCLUDE=(
*.[ch]pp
*.[ch]
*.am
*.y
*.qx
*.qrc
*.sh
)

GLOBS_EXCLUDE=(
Lexer.cpp
Lexer-token_ids.hpp
Lexer-token.hpp
Lexer-configuration.hpp
Parser.c
Parser.h
)

DIRS_EXCLUDE=(
ThirdParty
quex
)
##################
if [[ -z "$1" ]]; then
  echo "Usage: $(basename $0) <search-root-dir>"
  exit 1
fi

GREP_ROOT="$1"
GREP_PATTERN="Copyright "
GREP_ARGS=""

for glob in ${GLOBS_INCLUDE[*]}; do
GREP_ARGS+=" --include=$glob"
done

for glob in ${GLOBS_EXCLUDE[*]}; do
GREP_ARGS+=" --exclude=$glob"
done

for dir in ${DIRS_EXCLUDE[*]}; do
GREP_ARGS+=" --exclude-dir=$dir"
done

FILELIST=$(cd $GREP_ROOT && grep -Lr $GREP_PATTERN . $GREP_ARGS)

if [[ $? -ne 0 ]]; then
  exit 1
fi

if [[ -z "$FILELIST" ]]; then
  exit 0
fi

echo '#'
echo '# The following files seem not to contain copyright information:'
echo '#'
echo -e "$FILELIST"

exit 1
