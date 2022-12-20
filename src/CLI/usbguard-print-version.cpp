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
  static std::string toHumanReadable(const int enabled)
  {
    return enabled ? "enabled" : "disabled";
  }

  int usbguard_print_version(__attribute__((unused)) int argc, __attribute__((unused)) char* argv[])
  {
    if (argc != 1) {
      return EXIT_FAILURE;
    }

    int audit = 0, libcapng = 0, seccomp = 0, systemd = 0, umockdev = 0;
    std::string crypto;
#ifdef HAVE_LINUX_AUDIT
    audit = 1;
#endif
#ifdef HAVE_LIBCAPNG
    libcapng = 1;
#endif
#ifdef HAVE_SECCOMP
    seccomp = 1;
#endif
#ifdef SYSTEMD_SUPPORT_ENABLED
    systemd = 1;
#endif
#ifdef HAVE_UMOCKDEV
    umockdev = 1;
#endif
#ifdef USBGUARD_USE_LIBGCRYPT
    crypto = "libgcrypt";
#elif USBGUARD_USE_LIBSODIUM
    crypto = "libsodium";
#elif USBGUARD_USE_OPENSSL
    crypto = "openssl";
#else
    crypto = "unknown";
#endif
    std::cout << "usbguard " << PACKAGE_VERSION << " compiled with:" << std::endl;
    std::cout << "  Linux audit support:    " << toHumanReadable(audit) << std::endl;
    std::cout << "  Libcapng support:       " << toHumanReadable(libcapng) << std::endl;
    std::cout << "  Seccomp support:        " << toHumanReadable(seccomp) << std::endl;
    std::cout << "  Systemd support:        " << toHumanReadable(systemd) << std::endl;
    std::cout << "  Umockdev support:       " << toHumanReadable(umockdev) << std::endl;
    std::cout << "  Crypto backend library: " << crypto << std::endl;
    return EXIT_SUCCESS;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
