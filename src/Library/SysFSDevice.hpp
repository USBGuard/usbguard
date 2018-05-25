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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "UEvent.hpp"

#include "usbguard/Typedefs.hpp"

#include <string>

namespace usbguard
{
  class SysFSDevice
  {
  public:
    SysFSDevice();
    SysFSDevice(const std::string& sysfs_path, bool without_parent = false);
    SysFSDevice(SysFSDevice&& device);
    ~SysFSDevice();
    SysFSDevice& operator=(SysFSDevice&& rhs_device);

    const std::string& getPath() const;
    const std::string& getName() const;
    const UEvent& getUEvent() const;
    const std::string& getParentPath() const;
    bool hasAttribute(const std::string& name) const;
    std::string readAttribute(const std::string& name, bool trim = false, bool optional = false) const;
    void setAttribute(const std::string& name, const std::string& value);
    int openAttribute(const std::string& name) const;

    void reload();

    static void setSysfsRoot(const std::string& sysfs_root);
    static const std::string& getSysfsRoot();
  private:
    void reloadUEvent();

    std::string _sysfs_path;
    std::string _sysfs_name;
    std::string _sysfs_parent_path;
    int _sysfs_dirfd;
    UEvent _uevent;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
