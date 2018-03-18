#!/bin/bash
#
#
# Copyright (C) 2018 Red Hat, Inc.
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
# LDAP sanity testing...

set -x

source "${USBGUARD_TESTLIB_BASH}" || exit 129
#source "../bash-testlib.sh" || exit 129

export SLAPD_DIR=""

if [ -f /etc/redhat-release ] 
then
    SLAPD_DIR="/etc/openldap"
else
    SLAPD_DIR="/etc/ldap"
fi

# function setup()
# {
#     sudo -n systemctl restart slapd
#     ${LDAP_UTIL} setup
# }

# function clear()
# {
#     rm -f $SLAPD_DIR/schema/usbguard.ldif
#     ${LDAP_UTIL} delete
#     sudo -n systemctl restart slapd
# }

# setup

# clear

exit 0