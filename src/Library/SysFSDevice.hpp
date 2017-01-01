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
//
#pragma once

#include "UEvent.hpp"
#include "Typedefs.hpp"

namespace usbguard
{
  class SysFSDevice
  {
    public:
      SysFSDevice();
      SysFSDevice(const String& sysfs_path);
      SysFSDevice(SysFSDevice&& device);
      ~SysFSDevice();
      SysFSDevice& operator=(SysFSDevice&& rhs_device);

      const String& getPath() const;
      const String& getName() const;
      const UEvent& getUEvent() const;
      const String& getParentPath() const;
      String readAttribute(const String& name, bool strip_last_null = false, bool optional = false) const;
      void setAttribute(const String& name, const String& value);
      int openAttribute(const String& name) const;

      void reload();
    private:
      void reloadUEvent();

      String _sysfs_path;
      String _sysfs_name;
      String _sysfs_parent_path;
      int _sysfs_dirfd;
      UEvent _uevent;
  };
} /* namespace usbguard */
