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
#include "SysIO.hpp"
#include "Common/Logging.hpp"

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
    int value_size;
    int fd = -1;

    value_size = snprintf(value_buffer,
			  sizeof value_buffer, "%d", value);

    if (value_size < 1) {
      log->error("Failed to convert value to it's string representation in sysIOWrite");
      return;
    }

    fd = open(path, O_WRONLY);
    
    if (fd < 0) {
      log->error("Failed to open {} for writing", path);
      return;
    }

    if (write(fd, value_buffer, (size_t)value_size) != (ssize_t)value_size) {
      log->error("Failed to write \"{}\" to {}: {}", value_buffer, path, errno);
      /* fallthrough */
    }

    close(fd);
    return;
  }

  ssize_t sysioWriteFileAt(DIR* dirfp, const std::string& relpath, char *buffer, size_t buflen)
  {
    const int fd = openat(dirfd(dirfp), relpath.c_str(), O_WRONLY);

    if (fd < 0) {
      log->debug("Cannot open {}: {}", relpath, errno);
      return -1;
    }

    errno = 0;
    const ssize_t bytes_written = write(fd, buffer, buflen);
    log->debug("Wrote {} bytes out of {}: errno {}", bytes_written, buflen, errno);
    close(fd);

    return bytes_written;
  }

  ssize_t sysioReadFileAt(DIR* dirfp, const std::string& relpath, char *buffer, size_t buflen)
  {
    const int fd = openat(dirfd(dirfp), relpath.c_str(), O_RDONLY);

    if (fd < 0) {
      log->debug("Cannot open {}: {}", relpath, errno);
      return -1;
    }

    errno = 0;
    const ssize_t bytes_read = read(fd, buffer, buflen);
    log->debug("Read {} bytes out of {}: errno: {}", bytes_read, buflen, errno);
    close(fd);

    return bytes_read;
  }

  void sysioSetAuthorizedDefault(bool state)
  {
    log->debug("Setting default authorized flag values on all USB controllers to {}", state);
    const char * const dir = "/sys/bus/usb/devices/";
    DIR *dirfp = opendir(dir);
    struct dirent *dent;

    if (!dirfp) {
      throw std::runtime_error("Cannot open USB devices /sys directory");
    }

    for (dent = readdir(dirfp); dent != nullptr; dent = readdir(dirfp)) {
      char buffer[3] = { 0, 0, 0 };
      const std::string devpath(dent->d_name);

      if (devpath.compare(0, 3, "usb") != 0) {
	log->debug("Skipping device {}: does not start with 'usb'", devpath);
	continue;
      }

      /* bDeviceClass */
      if (sysioReadFileAt(dirfp, devpath + "/bDeviceClass", buffer, sizeof buffer) < 2) {
	log->debug("Skipping device {}: cannot read device class", devpath);
	continue;
      }
      if (!(buffer[0] == '0' && buffer[1] == '9')) {
	log->debug("Skipping device {}: wrong class value: {}{}", devpath, buffer[0], buffer[1]);
	continue;
      }
      /* bDeviceSubClass */
      if (sysioReadFileAt(dirfp, devpath + "/bDeviceSubClass", buffer, sizeof buffer) < 2) {
	log->debug("Skipping device {}: cannot read device subclass", devpath);
	continue;
      }
      if (!(buffer[0] == '0' && buffer[1] == '0')) {
	log->debug("Skipping device {}: wrong subclass value: {}{}", devpath, buffer[0], buffer[1]);
	continue;
      }

      buffer[0] = state ? '1' : '0';

      if (sysioWriteFileAt(dirfp, devpath + "/authorized_default",
			   buffer, 1) != 1) {
	log->debug("Cannot set default authorized state for device {}", devpath);
	continue;
      }
    }

    closedir(dirfp);
    return;
  }

} /* namespace usbguard */
