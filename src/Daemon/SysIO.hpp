#ifndef USBFW_SYSIO_HPP
#define USBFW_SYSIO_HPP

#include "Common/CCBQueue.hpp"
#include <limits.h>

namespace usbguard
{
  #ifndef SYSIO_SYSROOT_DEFAULT
  # define SYSIO_SYSROOT_DEFAULT "/sys"
  #endif

  #ifndef PATH_MAX
  # define PATH_MAX 256
  #endif

  #define SYSIO_REQUEST_READ  0
  #define SYSIO_REQUEST_WRITE 1

  #define SYSIO_PATH_MAX PATH_MAX

  struct SysIORequest
  {
    int type;
    int value;
    char path[SYSIO_PATH_MAX];
  };

  typedef CCBQueue<SysIORequest> SysIOQueue;

  void sysIOWrite(const char *path, int value);

} /* namespace usbguard */

#endif /* USBFW_SYSIO_HPP */
