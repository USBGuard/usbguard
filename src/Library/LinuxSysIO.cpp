//
// Copyright (C) 2015 Red Hat, Inc.
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
#include "LinuxSysIO.hpp"
#include "Logger.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

namespace usbguard
{
  void sysioWrite(const char *path, int value)
  {
    char value_buffer[3];
    const int value_size = snprintf(value_buffer, sizeof value_buffer, "%d", value);

    if (value_size < 1 || (size_t)value_size >= sizeof value_buffer) {
      USBGUARD_LOG(Error) << "sysioWrite: Failed to convert integer value to string";
      return;
    }

    const int fd = open(path, O_WRONLY);

    if (fd < 0) {
      USBGUARD_LOG(Error) << "sysioWrite: Failed to open " << path << ": errno=" << errno;
      return;
    }
    if (write(fd, value_buffer, (size_t)value_size) != (ssize_t)value_size) {
      USBGUARD_LOG(Error) << "sysioWrite: Unable to write to " << path << ": errno=" << errno;
      /* FALLTHROUGH */
    }

    (void)close(fd);
  }

  ssize_t sysioWriteFileAt(DIR* dirfp, const std::string& relpath, char *buffer, size_t buflen)
  {
    const int fd = openat(dirfd(dirfp), relpath.c_str(), O_WRONLY);

    if (fd < 0) {
      USBGUARD_LOG(Error) << "sysioWriteFileAt: Cannot open " << relpath << ": errno=" << errno;
      return -1;
    }

    errno = 0;
    const ssize_t bytes_written = write(fd, buffer, buflen);
    close(fd);

    return bytes_written;
  }

  ssize_t sysioReadFileAt(DIR* dirfp, const std::string& relpath, char *buffer, size_t buflen)
  {
    const int fd = openat(dirfd(dirfp), relpath.c_str(), O_RDONLY);

    if (fd < 0) {
      USBGUARD_LOG(Error) << "sysioReadFileAt: Cannot open " << relpath << ": errno=" << errno;
      return -1;
    }

    errno = 0;
    const ssize_t bytes_read = read(fd, buffer, buflen);
    close(fd);

    return bytes_read;
  }

  void sysioSetAuthorizedDefault(bool state)
  {
    const char * const dir = "/sys/bus/usb/devices/";
    DIR *dirfp = opendir(dir);
    struct dirent *dent;

    USBGUARD_LOG(Info) << "Setting default authorized flag values on all USB controllers to " << state;
 
    if (!dirfp) {
      throw std::runtime_error("Cannot open USB devices /sys directory");
    }

    for (dent = readdir(dirfp); dent != nullptr; dent = readdir(dirfp)) {
      char buffer[3] = { 0, 0, 0 };
      const std::string devpath(dent->d_name);

      if (devpath == "." || devpath == "..") {
        continue;
      }
      if (devpath.compare(0, 3, "usb") != 0) {
        USBGUARD_LOG(Debug) << "Skipping devpath=" << devpath << ":"
                            << " Does not start with 'usb'";
	continue;
      }

      /* bDeviceClass */
      if (sysioReadFileAt(dirfp, devpath + "/bDeviceClass", buffer, sizeof buffer) < 2) {
	USBGUARD_LOG(Warning) << "Cannot set default authorization state for devpath=" << devpath << ":"
                              << " Unable to read device class";
	continue;
      }
      if (!(buffer[0] == '0' && buffer[1] == '9')) {
	USBGUARD_LOG(Debug) << "Skipping devpath=" << devpath << ":"
                            << " Wrong class value";
	continue;
      }
      /* bDeviceSubClass */
      if (sysioReadFileAt(dirfp, devpath + "/bDeviceSubClass", buffer, sizeof buffer) < 2) {
	USBGUARD_LOG(Warning) << "Cannot set default authorization state for devpath=" << devpath << ":"
                              << " Unable to read device subclass.";
	continue;
      }
      if (!(buffer[0] == '0' && buffer[1] == '0')) {
	USBGUARD_LOG(Debug) << "Skipping devpath=" << devpath << ":"
                            << " Wrong subclass value.";
	continue;
      }

      buffer[0] = state ? '1' : '0';

      if (sysioWriteFileAt(dirfp, devpath + "/authorized_default",
			   buffer, 1) != 1) {
        USBGUARD_LOG(Warning) << "Cannot set default authorization state for devpath=" << devpath << ":"
                              << " Write operation failed.";
	continue;
      }
      USBGUARD_LOG(Info) << "Set default authorization state for devpath=" << devpath << " to " << state;
    }

    closedir(dirfp);
  }

} /* namespace usbguard */
