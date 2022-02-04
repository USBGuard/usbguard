#!/bin/bash
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
# Test the authorization of devices using umockdev.
#
source "${USBGUARD_TESTLIB_BASH}" || exit 129

# Skip if udevmock-wrapper is not available
command -v umockdev-wrapper || exit 77

#set -x

# TODO? Move to testlib
export USBGUARD_TESTLIB_TMPDIR="$(mktemp -d --tmpdir usbguard-test.XXXXXX)"

export config_path="${USBGUARD_TESTLIB_TMPDIR}/daemon.conf"
export policy_path="${USBGUARD_TESTLIB_TMPDIR}/policy.conf"

function test_cli_devices()
{
  set -e
  sleep 4

  export USBGUARD_DEBUG=1
  
  ${USBGUARD} list-devices
  ${USBGUARD} list-devices -a
  ${USBGUARD} list-devices -b

  local id_dock="$(${USBGUARD} list-devices | sed -n 's|^\([0-9]\+\):.*hash "2y2qS3rcuMr1Ye5knWsbD8CGzPtrs+eiRR/haro7+Ng=".*$|\1|p')"

  if [ -z "$id_dock" ]; then
    echo "Test error: Unable to parse device ID (test line ${LINENO})"
    exit 1
  fi


  ${USBGUARD} block-device "$id_dock"
  ${USBGUARD} allow-device "$id_dock"
  sleep 1

  local id_dock_child="$(${USBGUARD} list-devices | sed -n 's|^\([0-9]\+\):.*hash "D3deklX12Ir3kJPfUZ5AQNwHeZn1bwtPkQkw6e+8B38=".*$|\1|p')"
  
  if [ -z "$id_dock_child" ]; then
    echo "Test error: Unable to parse device ID (test line ${LINENO})"
    exit 1
  fi

  ${USBGUARD} block-device "$id_dock_child"
  ${USBGUARD} block-device "$id_dock"
  ${USBGUARD} allow-device "$id_dock"
  sleep 1

  local id_dock_child="$(${USBGUARD} list-devices | sed -n 's|^\([0-9]\+\):.*hash "D3deklX12Ir3kJPfUZ5AQNwHeZn1bwtPkQkw6e+8B38=".*$|\1|p')"
  
  if [ -z "$id_dock_child" ]; then
    echo "Test error: Unable to parse device ID (test line ${LINENO})"
    exit 1
  fi

  ${USBGUARD} allow-device "$id_dock_child"
  ${USBGUARD} list-devices

  set +e
  return 0
}

cat > "$config_path" <<EOF
RuleFile=$policy_path
ImplicitPolicyTarget=block
PresentDevicePolicy=apply-policy
PresentControllerPolicy=allow
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
cp "${srcdir}/src/Tests/UseCase/devices.umockdev" "${USBGUARD_UMOCKDEV_DEVICEDIR}"

schedule "umockdev-wrapper ${USBGUARD_DAEMON} -d -k -P -c $config_path" :service
schedule "test_cli_devices"
execute 60
retval=$?

rm -rf "$USBGUARD_UMOCKDEV_DEVICEDIR"

exit $retval
