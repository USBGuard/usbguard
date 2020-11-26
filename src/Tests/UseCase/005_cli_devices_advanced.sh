#!/bin/bash
#
# Copyright (C) 2020 Red Hat, Inc.
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
# Authors: Attila Lakatos <alakatos@redhat.com>
#
# Test CLI apply-device-policy.
#
source "${USBGUARD_TESTLIB_BASH}" || exit 129

# Skip if udevmock-wrapper is not available
command -v umockdev-wrapper || exit 77

#set -x

# TODO? Move to testlib
export USBGUARD_TESTLIB_TMPDIR="$(mktemp -d --tmpdir usbguard-test.XXXXXX)"

export config_path="${USBGUARD_TESTLIB_TMPDIR}/daemon.conf"
export policy_path="${USBGUARD_TESTLIB_TMPDIR}/policy.conf"


function test_cli_devices_advanced()
{
  set -e
  sleep 4

  export USBGUARD_DEBUG=1

  ${USBGUARD} list-devices
  ${USBGUARD} list-devices -a
  ${USBGUARD} list-devices -b


  #
  # Test case: usbguard (allow|block|reject)-device rule is specified in multiple arguments
  #
  ${USBGUARD} allow-device allow
  ${USBGUARD} allow-device block

  ${USBGUARD} allow-device match name \"Test\"
  ${USBGUARD} block-device match name \"Test\"
  ${USBGUARD} reject-device match name \"Test\"
  ${USBGUARD} allow-device match id 4321:"*"
  ${USBGUARD} block-device match id 4321:"*"
  ${USBGUARD} reject-device match id 4321:"*"
  ${USBGUARD} allow-device match id 1532:006e serial \"\" name \"Razer DeathAdder Essential\" via-port \"1-4.4.3\" with-interface \{ 03:01:02 03:00:01 03:00:01 \} with-connect-type \"unknown\"


  #
  # Test case: usbguard (allow|block|reject)-device partial rule is specified in multiple arguments
  #
  ${USBGUARD} allow-device name \"Test\"
  ${USBGUARD} block-device name \"Test\"
  ${USBGUARD} reject-device name \"Test\"
  ${USBGUARD} allow-device id 4321:"*"
  ${USBGUARD} block-device id 4321:"*"
  ${USBGUARD} reject-device id 4321:"*"
  ${USBGUARD} allow-device id 1532:006e serial \"\" name \"Razer DeathAdder Essential\" via-port \"1-4.4.3\" with-interface \{ 03:01:02 03:00:01 03:00:01 \} with-connect-type \"unknown\"


  #
  # Test case: usbguard (allow|block|reject)-device rule is specified in one argument
  #
  ${USBGUARD} allow-device 'match name "Test"'
  ${USBGUARD} block-device 'match name "Test"'
  ${USBGUARD} reject-device 'match name "Test"'
  ${USBGUARD} allow-device 'match id 4321:*'
  ${USBGUARD} block-device 'match id 4321:*'
  ${USBGUARD} reject-device 'match id 4321:*'
  ${USBGUARD} allow-device 'match id 1532:006e serial "" name "Razer DeathAdder Essential" via-port "1-4.4.3" with-interface { 03:01:02 03:00:01 03:00:01 } with-connect-type "unknown"'

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
schedule "test_cli_devices_advanced"
execute 60
retval=$?

rm -rf "$USBGUARD_UMOCKDEV_DEVICEDIR"

exit $retval
