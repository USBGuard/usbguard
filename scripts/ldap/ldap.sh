#!/bin/bash

set -e

BASE="dc=example,dc=com"
USER="cn=Manager,$BASE"

HOST="192.168.122.83"
PASS="passme"

ping -q -c 1 $HOST 2>&1 >/dev/null

if [ "$1" = "delete" ]; then
  ldapdelete -H "ldap://${HOST}/" -D $USER -w $PASS -r $BASE
  exit
fi

if [ "$1" = "setup" ]; then
  ldapadd -v -H "ldap://${HOST}/" -D $USER -w $PASS -f ./setup.ldif
  exit
fi

if [ "$1" = "policy" ]; then
  POLICY='./usbguard-policy.ldif'
  if [ "$2" ]; then
    POLICY=$2
  fi
  ldapadd -v -H "ldap://${HOST}/" -D $USER -w $PASS -f $POLICY
  exit
fi

if [ "$1" = "search" ]; then
  FILTER='objectclass=*'
  if [ "$2" ]; then
    FILTER=$2
  fi
  ldapsearch -H "ldap://${HOST}/" -D $USER -b $BASE -w $PASS $FILTER
  exit
fi
