#pragma once

#include "Common/CCBQueue.hpp"
#include <limits.h>
#include <dirent.h>
#include <string>

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

  void sysioWrite(const char *path, int value);
  ssize_t sysioWriteFileAt(DIR* dirfp, const std::string& relpath, char *buffer, size_t buflen);
  ssize_t sysioReadFileAt(DIR* dirfp, const std::string& relpath, char *buffer, size_t buflen);
  void sysioSetAuthorizedDefault(bool state);

} /* namespace usbguard */
