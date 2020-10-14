//
// Copyright (C) 2016 Red Hat, Inc.
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
//          Attila Lakatos <alakatos@redhat.com>
//
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard.hpp"
#include "usbguard-allow-device.hpp"
#include "usbguard-apply-device-policy.hpp"

#include <iostream>

namespace usbguard
{
  int usbguard_allow_device(int argc, char* argv[])
  {
    return usbguard_apply_device_policy(argc, argv, Rule::Target::Allow);
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
