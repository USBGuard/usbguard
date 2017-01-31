#!/bin/bash
#
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
# Test whether the binaries are executable as expected (no linker errors, etc.)
#
source "${USBGUARD_TESTLIB_BASH}" || exit 129

schedule "${USBGUARD}"
schedule "${USBGUARD}" :valgrind

schedule "${USBGUARD_DAEMON} -h"
schedule "${USBGUARD_DAEMON} -h" :valgrind

[ -f "${USBGUARD_DBUS}" ] && schedule "${USBGUARD_DBUS} -h"
[ -f "${USBGUARD_DBUS}" ] && schedule "${USBGUARD_DBUS} -h" :valgrind

execute 10
