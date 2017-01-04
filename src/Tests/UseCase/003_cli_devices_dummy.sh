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

#set -x

# TODO? Move to testlib
export USBGUARD_TESTLIB_TMPDIR="$(mktemp -d --tmpdir usbguard-test.XXXXXX)"

export config_path="${USBGUARD_TESTLIB_TMPDIR}/daemon.conf"
export policy_path="${USBGUARD_TESTLIB_TMPDIR}/policy.conf"

function test_cli_devices()
{
  set -e
  sleep 4

  ${USBGUARD} list-devices
  ${USBGUARD} list-devices -a
  ${USBGUARD} list-devices -b

  local id="$(${USBGUARD} list-devices | sed -n 's|^\([0-9]\+\):.*hash "FSgk48/lKiTJWdqOqkHLuMQr155m+ux+ozIb17HHcKs=".*$|\1|p')"

  if [ -z "$id" ]; then
    echo "Test error: Unable to find/parse device ID"
    exit 1
  fi

  ${USBGUARD} block-device "$id"
  ${USBGUARD} allow-device "$id"
  ${USBGUARD} block-device "$id"

  set +e
  return 0
}

cat > "$config_path" <<EOF
RuleFile=$policy_path
ImplicitPolicyTarget=allow
PresentDevicePolicy=allow
PresentControllerPolicy=allow
InsertedDevicePolicy=apply-policy
RestoreControllerDeviceState=false
DeviceManagerBackend=dummy
IPCAllowedUsers=$(id -un)
IPCAllowedGroups=$(id -gn)
DeviceRulesWithPort=false
EOF

cat > "$policy_path" <<EOF
EOF

rm -f /tmp/usbguard-dummy.sock
set +e
export USBGUARD_DUMMY_DEVICE_ROOT=/tmp/usbguard-dummy
mkdir "$USBGUARD_DUMMY_DEVICE_ROOT"
tar Jxvf "${srcdir}/src/Tests/UseCase/DummyDevices/root.tar.xz" -C "${USBGUARD_DUMMY_DEVICE_ROOT}"
set -e

schedule "${USBGUARD_DAEMON} -d -k -c $config_path" :service
schedule "test_cli_devices"
execute 60
retval=$?

rm -f /tmp/usbguard-dummy.sock
#rm -rf "$USBGUARD_DUMMY_DEVICE_ROOT"

exit $retval
