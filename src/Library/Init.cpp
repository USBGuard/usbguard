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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "IPCServerPrivate.hpp"
#include "IPCClientPrivate.hpp"

#if defined(USBGUARD_USE_LIBSODIUM)
  #include <sodium.h>
#endif
#if defined(USBGUARD_USE_LIBGCRYPT)
  #include <gcrypt.h>
#endif

#include <stdexcept>
#include <clocale>

namespace usbguard
{
  class LibraryInit
  {
  public:
    LibraryInit()
    {
      std::setlocale(LC_NUMERIC, "C");
      /*
       * FIXME: Provide a mechanism to disable the crypto library
       * initialization as it might interfere with initialization
       * done by third-party applications.
       *
       * gcrypt: whether initialization was performed or not can
       * be queried using:
       *  gcry_control(GCRYCTL_INITIALIZATION_FINISHED_P, 0);
       */
#if defined(USBGUARD_USE_LIBSODIUM)

      if (sodium_init() == -1) {
        throw std::runtime_error("Cannot initialize the sodium library");
      }

#endif
#if defined(USBGUARD_USE_LIBGCRYPT)

      if (!gcry_check_version(GCRYPT_VERSION)) {
        throw std::runtime_error("Cannot initialize the gcrypt library");
      }

      gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);
#endif
    }

    ~LibraryInit()
    {
      google::protobuf::ShutdownProtobufLibrary();
    }
  };

  static LibraryInit library_init;
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
