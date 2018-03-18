#!/bin/bash

set -e

BASE="dc=example,dc=com"
USER="cn=Manager,$BASE"

HOST="127.0.0.1"
PASS="passme"

#ping -q -c 1 $HOST 2>&1 >/dev/null

if [ "$1" = "delete" ]; then
  ldapdelete -h $HOST -D $USER -w $PASS -r $BASE
  exit
fi

if [ "$1" = "setup" ]; then
  cp ${USBGUARD_LDIF} $SLAPD_DIR/schema/
  ldapadd -Q -Y EXTERNAL -H ldapi:/// -f $SLAPD_DIR/schema/usbguard.ldif
  ldapadd -v -h $HOST -D $USER -w $PASS <<EOF
dn: dc=example,dc=com
objectclass: dcObject
objectclass: organization
dc: test
o: "Test server"

dn: ou=USBGuard,dc=example,dc=com
objectClass: top
objectClass: organizationalUnit
ou: USBGuard
EOF

  exit
fi

if [ "$1" = "policy" ]; then
#  POLICY='$(top_srcdir)/src/Tests/LDAP/usbguard-policy.ldif'
  if [ "$2" ]; then
    POLICY=$2
  fi
  ldapadd -v -h $HOST -D $USER -w $PASS -f $POLICY
  exit
fi

if [ "$1" = "search" ]; then
  FILTER='objectclass=*'
  if [ "$2" ]; then
    FILTER=$2
  fi
  ldapsearch -h $HOST -D $USER -b $BASE -w $PASS $FILTER
  exit
fi
