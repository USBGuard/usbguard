#!/bin/sh
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
set -x

mkdir doc/
gdbus-codegen --generate-docbook=doc DBusInterface.xml
mv doc-*.xml doc/
pushd doc/

echo "USBGuard DBus Interface Documentation"  > usbguard-dbus.adoc
echo "=====================================" >> usbguard-dbus.adoc
echo "" >> usbguard-dbus.adoc

for doc in doc-*.xml; do \
 docbook2html -u $doc

 html_file=$(echo $doc | sed 's/\.xml$/.html/')
 html_title=$(echo $doc | sed -n 's/^doc-\(.*\)\.xml$/\1/p')
 echo "* link:$html_file[$html_title]" >> usbguard-dbus.adoc
done
a2x -f xhtml usbguard-dbus.adoc
popd
