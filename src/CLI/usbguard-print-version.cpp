//
// Copyright (C) 2022 Red Hat, Inc.
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
// Authors: Attila Lakatos <alakatos@redhat.com>
//
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard.hpp"
#include "usbguard-print-version.hpp"

#include "usbguard/IPCClient.hpp"

#include <iostream>
#include <map>
#include <vector>

namespace usbguard
{
  int usbguard_print_version(__attribute__((unused)) int argc, __attribute__((unused)) char* argv[])
  {
    if (argc != 1) {
      return EXIT_FAILURE;
    }

    std::cout << "usbguard " << PACKAGE_VERSION << " compiled with:" << std::endl;
    std::cout << "\t" << "Platform: \t\t\t" << PLATFORM_ID << std::endl;
    std::cout << "\t" << "Platform (lsb_release -d)" << "\t" << PLATFORM_ID_LSB << std::endl;
#ifdef HAVE_LINUX_AUDIT
    std::cout << "\t" << "Linux audit support:\t\t" << HAVE_LINUX_AUDIT << std::endl;
#endif
#ifdef HAVE_LIBCAPNG
    std::cout << "\t" << "Libcapng support:\t\t" << HAVE_LIBCAPNG << std::endl;
#endif
#ifdef HAVE_SECCOMP
    std::cout << "\t" << "Seccomp support:\t\t" << HAVE_SECCOMP << std::endl;
#endif
#ifdef SYSTEMD_SUPPORT_ENABLED
    std::cout << "\t" << "Systemd support:\t\t" << SYSTEMD_SUPPORT_ENABLED << std::endl;
#endif
#ifdef HAVE_UMOCKDEV
    std::cout << "\t" << "Umockdev support:\t\t" << HAVE_UMOCKDEV << std::endl;
#endif
    std::string crypto;
#ifdef USBGUARD_USE_LIBGCRYPT
    crypto = "libgcrypt";
#elif USBGUARD_USE_LIBSODIUM
    crypto = "libsodium";
#elif USBGUARD_USE_OPENSSL
    crypto = "openssl";
#else
    crypto = "unknown";
#endif
    std::cout << "\t" << "Crypto backend library:\t\t" << crypto << std::endl;
    return EXIT_SUCCESS;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
