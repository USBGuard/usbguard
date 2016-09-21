#!/bin/bash
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

# uncomment for debugging
set -x

COMMAND_QUEUE=()
COMMAND_JOBID=()
COMMAND_STATE=()
COMMAND_SETUP=()
COMMAND_OFILE=()

function schedule()
{
  local command="$1"
  local setup="$2"

  COMMAND_QUEUE[${#COMMAND_QUEUE[@]}]="$command"
  COMMAND_JOBID[${#COMMAND_JOBID[@]}]=0
  COMMAND_STATE[${#COMMAND_STATE[@]}]=0
  COMMAND_SETUP[${#COMMAND_SETUP[@]}]="$setup"
  COMMAND_OFILE[${#COMMAND_OFILE[@]}]="/dev/null"
}

#
# setup: :<flag>[:<flag>...]
#
# service ... indicates that the command starts a long
#             running service requiring an explicit
#             action/command to shutdown gracefully.
#
# sudo ... run via `sudo -n`
#
# fail ... non-zero command return code is required (expected)
#

#
# state: <n>
#
# 0 ... not started
# 1 ... started
# 2 ... failed
# 3 ... finished
#

#
# Handle child processes. Save exit code in COMMAND_
#
function handle_SIGCHLD()
{
 for j in $(jobs -n); do
   echo "Job: $j"
 done
}

function execute()
{
  local timeout=${1:=30}
  local job_count=${#COMMAND_QUEUE[@]}
  local job_start=$(date +%s)
  local job_rc=0

  set +m
  # Setup SIGCHLD trap
  trap handle_SIGCHLD SIGCHLD

  # Execute commands in background
  for ((i = 0; i < $job_count; ++i)) {
     local setup="${COMMAND_SETUP[$i]}"
     local sudocmd=""

     if echo "$setup" | grep ':sudo'; then
       sudocmd="sudo -n "
     fi

     local command="$sudocmd ${COMMAND_QUEUE[$i]}"
     local logfile="$(mktemp --tmpdir usbguard-test-${job_start}_job_${i}.XXXXXX.log)"
     $command 2>&1 > "$logfile" &
     COMMAND_JOBID[$i]="$!"
     COMMAND_STATE[$i]="1"
     COMMAND_OFILE[$i]="$logfile"
  }

  # Wait for jobs, check timeout
  while true; do
    local jobwait=no
    local joblist="$(jobs -lr)"
    local curtime="$(date +%s)"

    #
    # For each command in the queue:
    #  1) check whether it is still running
    #  2) if non-service commands are still running
    #
    for ((i = 0; i < $job_count; ++i)) do
       local jobid="${COMMAND_JOBID[$i]}"
       local setup="${COMMAND_SETUP[$i]}"

       #
       # Check whether the job is still running
       #
       if echo "$joblist" | grep -q "$jobid"; then
         if ! echo "$setup" | grep -q ":service"; then
           jobwait=yes
         fi
       fi
    done

    if [ "x$jobwait" = "xyes" ]; then
      #
      # Check test timeout
      #
      if (( (curtime - job_start) > timeout)); then
        break
      else
        sleep 1
      fi
    else
      break
    fi
  done

  #
  # Cleanup jobs, stop service jobs
  #
  for ((i = 0; i < $job_count; ++i)) do
    local jobid="${COMMAND_JOBID[$i]}"
    local setup="${COMMAND_SETUP[$i]}"
    local logfile="${COMMAND_OFILE[$i]}"
    local sudocmd=""

    if echo "$setup" | grep -q ':sudo'; then
      sudocmd="sudo -n"  
    fi

    if echo "$setup" | grep -q ":service"; then
      for p in $(pgrep -P "$jobid") "$jobid"; do
        $sudocmd kill -SIGTERM "$p"
      done
      local waittime=0
      while true; do
        local joblist="$(jobs -lr)"
        if ! echo "$joblist" | grep -q "$jobid"; then
          wait "$jobid"
          if [ $? -ne 0 ]; then
            job_rc=1
          fi
          rm -f "$logfile"
          break
        else
          sleep 1
          waittime=$((waittime + 1))
          if (( waittime > 10 )); then
            $sudocmd kill -SIGKILL "$jobid"
            wait "$jobid"
            job_rc=1
            break
          fi
        fi
      done
    else
      wait "$jobid"
      if [ $? -ne 0 ]; then
        job_rc=1
      else
        rm -f "$logfile"
      fi
    fi
  done

  return $job_rc
}


