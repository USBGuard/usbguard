#!/bin/sh
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
TARGET=(allow block reject)

function generate_target {
  echo -n ${TARGET[$RANDOM % 3]}
}

OPERATORS=(equals one-of none-of all-of equals-ordered match-all)

function generate_hex {
  size=$1
  cat /dev/urandom | tr -d -c '[:xdigit:]' | head -c $size
}

function generate_interface_type {
  case $(( RANDOM % 3 )) in
    0)
      generate_hex 2
      echo -n ':'
      generate_hex 2
      echo -n ':'
      generate_hex 2
      ;;
    1)
      generate_hex 2
      echo -n ':'
      generate_hex 2
      echo -n ':*'
      ;;
    2)
      generate_hex 2
      echo -n ':*:*'
      ;;
  esac
}

function generate_device_id {
  generate_hex 4
  echo -n ":"
  (( RANDOM % 2 )) && generate_hex 4 || echo -n '*'
}

function generate_string {
  if [[ -n "$1" ]]; then
    size=$1
  else
    size=8
  fi
  cat /dev/urandom | tr -d -c '[:print:]' | tr -d '"\\()' | head -c $size
}

function generate_dq_string {
  echo -n '"'
  generate_string
  echo -n '"'
}

function generate_operator {
  echo -n ${OPERATORS[$RANDOM % 5]}
}

function generate_set {
  if [ -n "$2" ]; then
    size=$2
  else
    size=$((RANDOM % 8 + 1))
  fi
  (( RANDOM % 2 )) && (generate_operator && echo -n " ")
  echo -n "{ "
  for i in $(seq $size); do
    $1
    echo -n " "
  done
  echo -n "}"
}

function generate_string_attribute {
  name=$1
  echo -n $name
  echo -n " "
  case $((RANDOM % 2)) in
    0)
      generate_dq_string
      ;;
    1)
      generate_set generate_dq_string
      ;;
  esac
}

function generate_interface_attribute {
  echo -n "with-interface"
  echo -n " "
  case $((RANDOM % 2)) in
    0)
      generate_interface_type
      ;;
    1)
      generate_set generate_interface_type
      ;;
  esac
}

function generate_id_attribute {
  echo -n "id"
  echo -n " "
  case $((RANDOM % 2)) in
    0)
      generate_device_id
      ;;
    1)
      generate_set generate_device_id
      ;;
  esac
}

generated=(0 0 0 0 0 0 0)

function generate_attribute {
  while true; do
    rnd=$((RANDOM % 7))
    if [ ${generated[$rnd]} -eq 0 ]; then
      generated[$rnd]=1
      break
    fi
  done
  case "$rnd" in
    0)
      generate_string_attribute "name"
      ;;
    1)
      generate_string_attribute "hash"
      ;;
    2)
      generate_string_attribute "parent-hash"
      ;;
    3)
      generate_string_attribute "serial"
      ;;
    4)
      generate_string_attribute "via-port"
      ;;
    5)
      generate_interface_attribute
      ;;
    6)
      generate_id_attribute
      ;;
  esac
}

function generate_condition {
  case $((RANDOM % 13)) in
    0)
      echo -n "if true"
      ;;
    1)
      echo -n "if !true"
      ;;
    2)
      echo -n "if false"
      ;;
    3)
      echo -n "if !false"
      ;;
    4)
      echo -n "if localtime(1:00)"
      ;;
    5)
      echo -n "if !localtime(2:00)"
      ;;
    6)
      echo -n "if "
      generate_operator
      echo -n " { true false !true false }"
      ;;
    8)
      echo -n "if random"
      ;;
    9)
      echo -n "if random(0.4)"
      ;;
    10)
      echo -n "if rule-applied"
      ;;
    11)
      echo -n "if rule-evaluated"
      ;;
    12)
      echo -n "if allowed-matches(name "
      generate_dq_string
      echo -n ")"
      ;;
  esac
}

function generate_rule {
  num_attributes=$1

  generate_target
  echo -n " "
  for i in $(seq $num_attributes); do
    generate_attribute
    echo -n " "
  done
  (( RANDOM % 2 )) && (echo -n ' '; generate_condition)
}

if [ -n "$1" ]; then
  num_attributes=$1
else
  num_attributes=$((RANDOM % 7 + 1))
fi

generate_rule $num_attributes
echo
