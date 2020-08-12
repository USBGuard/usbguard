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
# Test the authorization of devices using the kernel g_mass_storage and
# dummy_hcd modules. This test must be run by a user that can elevate
# their privileges using sudo.
#
source "${USBGUARD_TESTLIB_BASH}" || exit 129

#
# Skip the test if the kernel doesn't export the USB
# device directory.
#
[ -d "/sys/bus/usb/devices" ] || exit 77

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

  local id="$(${USBGUARD} list-devices | sed -n 's|^\([0-9]\+\):.*serial "555666111".*$|\1|p')"

  if [ -z "$id" ]; then
    echo "Test error: Unable to find/parse device ID"
    exit 1
  fi

  ${USBGUARD} block-device "$id"
  ${USBGUARD} allow-device "$id"
  ${USBGUARD} block-device "$id"
  ${USBGUARD} reject-device "$id"

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
DeviceManagerBackend=uevent
IPCAllowedUsers=$(id -un)
IPCAllowedGroups=$(id -gn)
DeviceRulesWithPort=false
EOF

cat > "$policy_path" <<EOF
EOF

# Create a dummy USB mass storage device
sudo -n dd bs=4096 count=1 if=/dev/zero of=/tmp/usbguard_disk
sudo -n modprobe dummy_hcd || exit 77 # Skip test if dummy_hcd is missing
sudo -n rmmod g_mass_storage
sudo -n modprobe g_mass_storage file=/tmp/usbguard_disk iSerialNumber=555666111

# Skip test if device creation fails
if [ $? -ne 0 ]; then
  exit 77
fi

schedule "${USBGUARD_DAEMON} -d -P -k -c $config_path" :service:sudo
schedule "test_cli_devices"
execute 60
retval=$?
sudo -n rmmod g_mass_storage
#sudo -n rmmod dummy_hcd
exit $retval
