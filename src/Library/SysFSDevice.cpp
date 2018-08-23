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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "SysFSDevice.hpp"
#include "Common/Utility.hpp"

#include "usbguard/Logger.hpp"
#include "usbguard/Exception.hpp"

#ifndef _POSIX_C_SOURCE
  #define _POSIX_C_SOURCE
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace usbguard
{
  static std::string G_sysfs_root = "/sys";

  void SysFSDevice::setSysfsRoot(const std::string& sysfs_root)
  {
    G_sysfs_root = sysfs_root;
  }

  const std::string& SysFSDevice::getSysfsRoot()
  {
    return G_sysfs_root;
  }

  SysFSDevice::SysFSDevice()
    : _sysfs_dirfd(-1)
  {
  }

  SysFSDevice::SysFSDevice(const std::string& sysfs_path, bool without_parent)
    : _sysfs_path(sysfs_path),
      _sysfs_name(filenameFromPath(_sysfs_path, /*include_extension=*/true)),
      _sysfs_dirfd(-1)
  {
    USBGUARD_LOG(Trace) << "sysfs_path=" << sysfs_path
      << " without_parent=" << without_parent;

    if (!without_parent) {
      _sysfs_parent_path = parentPath(sysfs_path);

      if (_sysfs_parent_path.empty()) {
        throw Exception("SysFSDevice", sysfs_path, "Cannot get parent path");
      }

      USBGUARD_LOG(Debug) << "parent_path=" << _sysfs_parent_path;
    }

    USBGUARD_SYSCALL_THROW("SysFSDevice",
      (_sysfs_dirfd = open((G_sysfs_root + _sysfs_path).c_str(), O_PATH|O_DIRECTORY)) < 0);

    try {
      reloadUEvent();
    }
    catch (...) {
      close(_sysfs_dirfd);
      throw;
    }
  }

  SysFSDevice::SysFSDevice(SysFSDevice&& device)
    : _sysfs_path(std::move(device._sysfs_path)),
      _sysfs_name(std::move(device._sysfs_name)),
      _sysfs_parent_path(std::move(device._sysfs_parent_path)),
      _uevent(std::move(device._uevent))
  {
    _sysfs_dirfd = device._sysfs_dirfd;
    device._sysfs_dirfd = -1;
  }

  SysFSDevice::~SysFSDevice()
  {
    if (_sysfs_dirfd != -1) {
      close(_sysfs_dirfd);
    }
  }

  SysFSDevice& SysFSDevice::operator=(SysFSDevice&& rhs_device)
  {
    _sysfs_path = std::move(rhs_device._sysfs_path);
    _sysfs_name = std::move(rhs_device._sysfs_name);
    _sysfs_parent_path = std::move(rhs_device._sysfs_parent_path);
    _sysfs_dirfd = std::move(rhs_device._sysfs_dirfd);
    rhs_device._sysfs_dirfd = -1;
    _uevent = std::move(rhs_device._uevent);
    return *this;
  }

  const std::string& SysFSDevice::getPath() const
  {
    return _sysfs_path;
  }

  const std::string& SysFSDevice::getName() const
  {
    return _sysfs_name;
  }

  const UEvent& SysFSDevice::getUEvent() const
  {
    return _uevent;
  }

  const std::string& SysFSDevice::getParentPath() const
  {
    return _sysfs_parent_path;
  }

  bool SysFSDevice::hasAttribute(const std::string& name) const
  {
    struct ::stat st;

    if (::fstatat(_sysfs_dirfd, name.c_str(), &st, AT_SYMLINK_NOFOLLOW) != 0) {
      if (errno == ENOENT) {
        return false;
      }

      throw ErrnoException("SysFSDevice::hasAttribute", name, errno);
    }

    return S_ISREG(st.st_mode);
  }

  int SysFSDevice::openAttribute(const std::string& name) const
  {
    USBGUARD_LOG(Trace) << "name=" << name;
    const int fd = openat(_sysfs_dirfd, name.c_str(), O_RDONLY);

    if (fd < 0) {
      throw ErrnoException("SysFSDevice", name, errno);
    }

    return fd;
  }

  std::string SysFSDevice::readAttribute(const std::string& name, bool trim, bool optional) const
  {
    USBGUARD_LOG(Trace) << "name=" << name;
    const ScopedFD fd(openat(_sysfs_dirfd, name.c_str(), O_RDONLY));

    if (fd < 0) {
      if (optional && errno == ENOENT) {
        return std::string();
      }
      else {
        throw ErrnoException("SysFSDevice", name, errno);
      }
    }

    std::string buffer(4096, 0);
    ssize_t rc = -1;
    USBGUARD_SYSCALL_THROW("SysFSDevice",
      (rc = read(fd, &buffer[0], buffer.capacity())) < 0);

    if (rc <= 0) {
      return std::string();
    }

    const size_t read_size = static_cast<size_t>(rc);

    if (trim) {
      size_t trimmed_size = read_size;

      while (trimmed_size > 0) {
        bool stop = false;

        switch (buffer[trimmed_size - 1]) {
        case '\0':
        case '\n':
        case '\r':
        case '\t':
        case '\b':
          break;

        default:
          stop = true;
        }

        if (stop) {
          break;
        }
        else {
          --trimmed_size;
        }
      }

      buffer.resize(trimmed_size);
    }
    else {
      buffer.resize(read_size);
    }

    return buffer;
  }

  void SysFSDevice::setAttribute(const std::string& name, const std::string& value)
  {
    USBGUARD_LOG(Trace) << "name=" << name << " value=" << value;
    USBGUARD_LOG(Trace) << "path=" << _sysfs_path;
    const ScopedFD fd(openat(_sysfs_dirfd, name.c_str(), O_WRONLY));

    if (fd < 0) {
      throw ErrnoException("SysFSDevice", name, errno);
    }

    ssize_t rc = -1;
    USBGUARD_SYSCALL_THROW("SysFSDevice",
      (rc = write(fd, &value[0], value.size())) != (ssize_t)value.size());
  }

  void SysFSDevice::reload()
  {
    reloadUEvent();
  }

  void SysFSDevice::reloadUEvent()
  {
    const std::string uevent_string = readAttribute("uevent");
    _uevent = UEvent::fromString(uevent_string, /*attributes_only=*/true);
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
