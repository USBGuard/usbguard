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
# Authors: Jiri Vymazal <jvymazal@redhat.com>
#
# Test the execution of the daemon with an empty configuration.
#
source "${USBGUARD_TESTLIB_BASH}" || exit 129

# TODO? Move to testlib
export USBGUARD_TESTLIB_TMPDIR="$(mktemp -d --tmpdir usbguard-test.XXXXXX)"

export config_path="${USBGUARD_TESTLIB_TMPDIR}/daemon.conf"
export pidfile_path="${USBGUARD_TESTLIB_TMPDIR}/usbguard.pid"
export logfile="${USBGUARD_TESTLIB_TMPDIR}/daemon.log"

function test_cli_daemonize()
{
  sleep 5

  if [ ! -f "$pidfile_path" ]; then
    echo "Test error: PID file for usbguard not present"
    exit 1
  fi

  if [ ! `pgrep usbguard` == `cat $pidfile_path` ]; then
    echo "Test error: PID of usbguard daemon not present in PID file"
    exit 1
  fi
}

cat > "$config_path" <<EOF
EOF

schedule "${USBGUARD_DAEMON} -f -p $pidfile_path -d -P -l $logfile -c $config_path" :service
schedule "test_cli_daemonize"
execute 20
retval=$?
cat $pidfile_path | xargs kill -9
exit $retval
