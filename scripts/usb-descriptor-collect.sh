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
set -e -o pipefail

SED=$(which sed)
FIND=$(which find)
LSUSB=$(which lsusb)
SHA1SUM=$(which sha1sum)
TAR=$(which tar)
MKTEMP=$(which mktemp)
CP=$(which cp)
WC=$(which wc)
RM=$(which rm)

TEMPDIR=$($MKTEMP -d --tmpdir usb-descriptor-collect.XXXXXX || (echo "Failed to create temporary directory"; exit 1))
ROOTDIR="$TEMPDIR/usb-descriptor-data"

mkdir $ROOTDIR || (echo "Failed to create data root directory: $ROOTDIR"; exit 1)

for dev_syspath in $($FIND /sys/bus/usb/devices/ -regex '.*/[0-9]+-[0-9]+\(\.[0-9]+\)*$' -o -regex '.*/usb[0-9]+$'); do
  echo "Collecting data from $dev_syspath"
  pushd "$dev_syspath"
  dev_bus=$(< busnum)
  dev_num=$(< devnum)
  echo " busnum: $dev_bus"
  echo " devnum: $dev_num"
  count_c=$($LSUSB -s $dev_bus:$dev_num -v 2> /dev/null | $SED -n 's|Configuration Descriptor|&|p' | $WC -l)
  count_i=$($LSUSB -s $dev_bus:$dev_num -v 2> /dev/null | $SED -n 's|Interface Descriptor|&|p' | $WC -l)
  count_e=$($LSUSB -s $dev_bus:$dev_num -v 2> /dev/null | $SED -n 's|Endpoint Descriptor|&|p' | $WC -l)
  echo " count_c: $count_c"
  echo " count_i: $count_i"
  echo " count_e: $count_e"
  descriptor_hash=$($SHA1SUM descriptors | $SED -n 's|^\([a-fA-F0-9]\{40\}\).*$|\1|p')
  echo " hash: $descriptor_hash"
  echo "count_c: $count_c" >  "$ROOTDIR/$descriptor_hash.log"
  echo "count_i: $count_i" >> "$ROOTDIR/$descriptor_hash.log"
  echo "count_e: $count_e" >> "$ROOTDIR/$descriptor_hash.log"
  $CP -f descriptors "$ROOTDIR/$descriptor_hash.bin"
  popd
done

ARCHIVE_PATH="$TEMPDIR/usb-descriptor-data.tar.gz"

echo "Creating data archive: $ARCHIVE_PATH"
pushd "$TEMPDIR"
$TAR zcvf $ARCHIVE_PATH usb-descriptor-data
popd
$RM -rf "$TEMPDIR/usb-descriptor-data"

echo "========================================="
echo
echo " Data collection complete!"
echo " Please send the data archive to dnk.usbdev@gmail.com"
echo
echo " Archive path: $ARCHIVE_PATH"
echo
echo " Thanks!"
echo
echo "========================================="

