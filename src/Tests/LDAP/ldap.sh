#!/bin/bash

#set -xe

BASE="dc=example,dc=com"
USER="cn=Manager,$BASE"

USBGUARD_BASE="ou=USBGuard,$BASE"

HOST="127.0.0.1"
PASS="passme"

#ping -q -c 1 $HOST 2>&1 >/dev/null

if [ "$1" = "delete" ]; then
  ldapdelete -H "ldap://${HOST}/" -D $USER -w $PASS -r $USBGUARD_BASE
  exit
fi

if [ "$1" = "setup" ]; then
  ldapadd -v -H "ldap://${HOST}/" -D $USER -w $PASS <<EOF
#dn: dc=example,dc=com
#objectclass: dcObject
#objectclass: organization
#dc: example
#o: "Test server"

dn: ou=USBGuard,dc=example,dc=com
objectClass: top
objectClass: organizationalUnit
ou: USBGuard
EOF

  exit
fi

if [ "$1" = "policy" ]; then
  POLICY=""
  if [ "$2" ]; then
    POLICY=$2
  else
    exit
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
