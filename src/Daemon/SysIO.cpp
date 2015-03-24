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
  void sysIOWrite(const char *path, int value)
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

} /* namespace usbguard */
