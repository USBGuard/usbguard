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
# Test the generation of policy based on umockdev.
#
source "${USBGUARD_TESTLIB_BASH}" || exit 129

#
# Skip the test if the kernel doesn't export the USB
# device directory.
#
[ -d "/sys/bus/usb/devices" ] || exit 77

# Skip if udevmock-wrapper is not available
command -v umockdev-wrapper || exit 77

#set -x

# TODO? Move to testlib
export USBGUARD_TESTLIB_TMPDIR="$(mktemp -d --tmpdir usbguard-test.XXXXXX)"

export config_path="${USBGUARD_TESTLIB_TMPDIR}/daemon.conf"
export policy_path="${USBGUARD_TESTLIB_TMPDIR}/policy.conf"

function test_cli_policy()
{
  set -e
  sleep 4
  
  c=$(${USBGUARD} list-rules | wc -l)
  # TODO [ $c -eq 0 ] || return 1

  ${USBGUARD} append-rule block
  
  c=$(${USBGUARD} list-rules | wc -l)
  # TODO [ $c -eq 1 ] || return 1

  ${USBGUARD} remove-rule 1
  
  c=$(${USBGUARD} list-rules | wc -l)
  # TODO [ $c -eq 0 ] || return 1

  ${USBGUARD} generate-policy
  ${USBGUARD} generate-policy -p
  ${USBGUARD} generate-policy -P
  ${USBGUARD} generate-policy -t block
  ${USBGUARD} generate-policy -t allow
  ${USBGUARD} generate-policy -t reject
  ${USBGUARD} generate-policy -X
  ${USBGUARD} generate-policy -H
  ${USBGUARD} generate-policy -h

  set +e
  return 0
}

cat > "$config_path" <<EOF
RuleFile=$policy_path
ImplicitPolicyTarget=block
PresentDevicePolicy=keep
PresentControllerPolicy=keep
InsertedDevicePolicy=apply-policy
RestoreControllerDeviceState=false
DeviceManagerBackend=umockdev
IPCAllowedUsers=$(id -un)
IPCAllowedGroups=$(id -gn)
DeviceRulesWithPort=false
EOF

cat > "$policy_path" <<EOF
EOF

export USBGUARD_UMOCKDEV_DEVICEDIR=/tmp/usbguard-dummy

rm -rf "$USBGUARD_UMOCKDEV_DEVICEDIR"
mkdir -p "$USBGUARD_UMOCKDEV_DEVICEDIR"

schedule "umockdev-wrapper ${USBGUARD_DAEMON} -d -k -P -c $config_path" :service
schedule "test_cli_policy"
execute 60
