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
# Authors: Radovan Sroka <rsroka@redhat.com>          
#


export TMPDIR=$(mktemp -d)

function nss_backup ()
{
    sudo -n cp /etc/nsswitch.conf ${TMPDIR}/
}

function nss_restore ()
{
    sudo -n cp ${TMPDIR}/nsswitch.conf /etc/nsswitch.conf
    sudo -n rm -rf ${TMPDIR}
}

function nss_remove ()
{
    sudo -n sed -ie '/^.*usbguard.*:.*/Id' /etc/nsswitch.conf
}

function nss_set ()
{
    nss_remove
    sudo -n sh -c  "echo \"usbguard: $1\" >> /etc/nsswitch.conf"
}