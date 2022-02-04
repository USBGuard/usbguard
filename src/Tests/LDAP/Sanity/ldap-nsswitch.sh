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


# Test how usbguard treats nsswitch.conf

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
    sudo -n mkdir -p /etc/usbguard
fi

if [ -f /etc/usbguard/usbguard-ldap.conf ]
then
    sudo -n sh -c "cp /etc/usbguard/usbguard-ldap.conf ${TMPDIR}/usbguard-ldap.conf.back"
    LDAP_CONF="yes"
fi

sudo -n cat > "${TMPDIR}/policy.ldif" <<EOF
dn: cn=Rule1,ou=USBGuard,dc=example,dc=com
objectClass: USBGuardPolicy
objectClass: top
cn: Rule1
USBGuardRuleTarget: allow
USBGuardHost: *
USBGuardRuleOrder: 1
EOF

${LDAP_UTIL} delete && true
${LDAP_UTIL} setup
${LDAP_UTIL} policy "${TMPDIR}/policy.ldif"

export config_path="${TMPDIR}/usbguard-daemon.conf"
export ldap_path="${TMPDIR}/usbguard-ldap.conf"

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

sudo -n cp "$ldap_path" /etc/usbguard/usbguard-ldap.conf

COUNTER="0"
declare -A BAD
PIDFILE="/var/run/usbguard.pid"

function grep_and_fail ()
{
    grep "$1" $TMPDIR/usbguard.log
    GREP=$?

    if ! [ $GREP -eq "0" ]
    then
        BAD[$COUNTER]="$1"
        COUNTER=$(( $COUNTER + 1 ))
        head -n 100 $TMPDIR/usbguard.log
    fi
}

lines=(
    "usbguard:files"
    "usbguard: files"
    "  usbguard  :  files"
    "  usbguard  :files"
    "USBGuard: files"
    "usbguard: FILES"
    " usbguard: Files "

    "usbguard:ldap"
    "usbguard: ldap"
    "  usbguard  :  ldap"
    "  usbguard  :ldap"
    "USBGuard: ldap"
    "usbguard: LDAP"
    " usbguard: Ldap "

    ""
    "usbguard: ddddddddddddddddddddd"
    "$(printf 'usbguard: files\nusbguard: ldap' )"
    "$(printf 'usbguard: ldap\nusbguard: files' )"  
)

results=(
    "Fetched value is -> FILES <-"
    "Fetched value is -> FILES <-"
    "Fetched value is -> FILES <-"
    "Fetched value is -> FILES <-"
    "Fetched value is -> FILES <-"
    "Fetched value is -> FILES <-"
    "Fetched value is -> FILES <-"

    "Fetched value is -> LDAP <-"
    "Fetched value is -> LDAP <-"
    "Fetched value is -> LDAP <-"
    "Fetched value is -> LDAP <-"
    "Fetched value is -> LDAP <-"
    "Fetched value is -> LDAP <-"
    "Fetched value is -> LDAP <-"

    "Value is not valid or not set, using default FILES"
    "Value is not valid or not set, using default FILES"
    "Fetched value is -> FILES <-"
    "Fetched value is -> LDAP <-"
)

# combinations ##########################################################################################
for i in `seq 0 $(( ${#lines[@]} - 1 ))`
do
    nss_remove
    sudo -n sh -c "echo \"${lines[$i]}\" >> /etc/nsswitch.conf"

    cat /etc/nsswitch.conf

    sudo -n ${USBGUARD_DAEMON} -Pd -f -c $config_path 2> $TMPDIR/usbguard.log

    echo "RC == $?"

    sleep 1

    echo "line: ${lines[$i]} | pattern: ${results[$i]}"

    grep_and_fail "${results[$i]}"

    # for sure, it should be already dead because there is no data in LDAP
    PID=$(sudo -n cat $PIDFILE)
    sudo -n kill $PID
    sudo -n rm -f $PIDFILE

done

# empty nsswitch ########################################################################################
sudo -n sh -c 'echo "" > /etc/nsswitch.conf'

sudo -n ${USBGUARD_DAEMON} -Pd -f -c $config_path 2> $TMPDIR/usbguard.log

echo "RC == $?"

sleep 1

PID=$(sudo -n cat $PIDFILE)
sudo -n kill $PID
rckilla=$?
sudo -n rm -f $PIDFILE

grep_and_fail "Value is not valid or not set, using default FILES"

# removed nsswitch #####################################################################################
sudo -n rm -rf /etc/nsswitch.conf

sudo -n ${USBGUARD_DAEMON} -Pd -f -c $config_path 2> $TMPDIR/usbguard.log

echo "RC == $?"

sleep 1

PID=$(sudo -n cat $PIDFILE)
sudo -n kill $PID
rckillb=$?
sudo -n rm -f $PIDFILE

grep_and_fail '(i) Error when opening nsswitch file: /etc/nsswitch.conf: No such file or directory'

if [ $rckillb -eq "1" ] # expected to fail
then
    rckillb="0"
fi

#######################################################################################################

${LDAP_UTIL} delete

if [ "$LDAP_CONF" == "yes" ]
then
    sudo -n sh -c "cp ${TMPDIR}/usbguard-ldap.conf.back /etc/usbguard/usbguard-ldap.conf"
fi
nss_restore

exit $(( ${#BAD[@]} + $rckilla + $rckillb ))
