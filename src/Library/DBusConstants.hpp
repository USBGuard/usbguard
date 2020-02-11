//
// Copyright (C) 2019 Red Hat, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Authors: Daniel Kopecek <dkopecek@redhat.com>
//          Allen Webb <allenwebb@google.com>
//
#pragma once

#define DBUS_API_VERSION "1"

#define DBUS_SERVICE_NAME      ("org.usbguard" DBUS_API_VERSION)
#define DBUS_ROOT_INTERFACE    ("org.usbguard" DBUS_API_VERSION)
#define DBUS_ROOT_PATH         ("/org/usbguard" DBUS_API_VERSION)
#define DBUS_POLICY_INTERFACE  ("org.usbguard.Policy" DBUS_API_VERSION)
#define DBUS_POLICY_PATH       ("/org/usbguard" DBUS_API_VERSION "/Policy")
#define DBUS_DEVICES_INTERFACE ("org.usbguard.Devices" DBUS_API_VERSION)
#define DBUS_DEVICES_PATH      ("/org/usbguard" DBUS_API_VERSION "/Devices")
