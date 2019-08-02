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


# Run USBGuard with empty ruleset
# with configured ldap

if [ $1 ]
then
    source "../../bash-testlib.sh" || exit 129
    LDAP_UTIL=../ldap.sh
    NSSWITCH=../nsswitch.sh
    USBGUARD=../../../../build/usbguard
    USBGUARD_DAEMON=../../../../build/usbguard-daemon
    USBGUARD_DBUS=../../../../build/usbguard-dbus
else
    source "${USBGUARD_TESTLIB_BASH}" || exit 129
fi

source ${NSSWITCH} || exit 129

#creates TMPDIR 
nss_backup
nss_set "ldap"

if ! [ -d /etc/usbguard ]
then
    sudo mkdir -p /etc/usbguard
fi 

if [ -f /etc/usbguard/usbguard-ldap.conf ]
then
    cp /etc/usbguard/usbguard-ldap.conf ${TMPDIR}/usbguard-ldap.conf.back
    LDAP_CONF="yes"
fi

export config_path="${TMPDIR}/usbguard-daemon.conf"
export ldap_path="${TMPDIR}/usbguard-ldap.conf"

# sudo -n cat > "${TMPDIR}/policy.ldif" <<EOF
# dn: cn=Rule1,ou=USBGuard,dc=example,dc=com
# objectClass: USBGuardPolicy
# objectClass: top
# cn: Rule1
# RuleType: allow
# USBGuardHost: *
# USBGuardOrder: 1
# EOF

sudo -n cat > "$config_path" <<EOF
ImplicitPolicyTarget=block
PresentDevicePolicy=keep
PresentControllerPolicy=keep
InsertedDevicePolicy=apply-policy
RestoreControllerDeviceState=false
DeviceManagerBackend=uevent
IPCAllowedUsers=$(id -un)
IPCAllowedGroups=$(id -gn)
DeviceRulesWithPort=false
EOF

sudo -n cat > "$ldap_path" <<EOF
URI ldap://127.0.0.1/
ROOTDN cn=Manager,dc=example,dc=com
base dc=example,dc=com
ROOTPW passme
EOF

sudo -n cat "$config_path"
sudo -n cat "$ldap_path"

sudo -n cp "$ldap_path" /etc/usbguard/usbguard-ldap.conf

PIDFILE="/var/run/usbguard.pid"

${LDAP_UTIL} delete && true
${LDAP_UTIL} setup
# ${LDAP_UTIL} policy "${TMPDIR}/policy.ldif"

sudo -n ${USBGUARD_DAEMON} -Pd -f -c $config_path 2> $TMPDIR/usbguard.log
RC=$?
echo "RC == $RC"

cat $TMPDIR/usbguard.log




PID=$(sudo -n cat $PIDFILE)
sudo -n kill $PID
KILLRC=$?
sudo -n rm -f $PIDFILE



grep "Finished daemonization" $TMPDIR/usbguard.log
GREP1=$?

grep -i "Sanitizer" $TMPDIR/usbguard.log
GREP2=$?

if [ "$GREP2" -eq "0" ]  # expected fail
then
    GREP2="1"
else
    GREP2="0"
fi

${LDAP_UTIL} delete

if [ "$LDAP_CONF" == "yes" ]
then
    sudo -n sh -c "cp ${TMPDIR}/usbguard-ldap.conf.back /etc/usbguard/usbguard-ldap.conf"
fi

nss_restore

exit $(( $RC + $GREP1 + $GREP2 + $KILLRC ))
