//
// Copyright (C) 2018 Red Hat, Inc.
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

#include <string>
#include <vector>
#include <memory>

namespace usbguard
{
  class UMockdevDeviceDefinition
  {
  public:
    UMockdevDeviceDefinition();
    UMockdevDeviceDefinition(UMockdevDeviceDefinition&&) = default;
    ~UMockdevDeviceDefinition() = default;

    const std::string& getUMockdevName() const;
    void setUMockdevName(const std::string& name);

    const std::string& getSysfsPath() const;
    void setSysfsPath(const std::string& sysfs_path);

    const std::string& getDevfsNode() const;
    void setDevfsNode(const std::string& devfs_node);
    bool hasDevfsNode() const;

    const std::string& getDeviceType() const;
    void setDeviceType(const std::string& device_type);

    const std::string& getUMockdevDefinition() const;
    void setUMockdevDefinition(const std::string& definition);
    operator bool () const;

    static std::vector<std::unique_ptr<UMockdevDeviceDefinition>> parseFromFile(const std::string& filepath,
        bool sort_by_hierarchy = false);
    static std::vector<std::unique_ptr<UMockdevDeviceDefinition>> parseFromString(const std::string& definitions,
        bool sort_by_hierarchy = false);

  private:
    std::string _name;
    std::string _sysfs_path;
    std::string _devfs_node;
    std::string _device_type;
    std::string _definition;
  };
} /* namespace usbguard */
/* vim: set ts=2 sw=2 et */
