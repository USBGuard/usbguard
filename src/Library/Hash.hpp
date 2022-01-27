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
#pragma once
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard/Typedefs.hpp"

#include <istream>
#include <string>

#include <cstddef>

#if defined(USBGUARD_USE_LIBSODIUM)
  #include <sodium.h>
#elif defined(USBGUARD_USE_OPENSSL)
  #include <openssl/evp.h>
#elif defined(USBGUARD_USE_LIBGCRYPT)
  #include <gcrypt.h>
#else
  #error "Don't know which crypto library to use."
#endif

namespace usbguard
{
  class Hash
  {
  public:
    Hash();
    Hash(const Hash& rhs);
    Hash(Hash&& rhs);
    Hash& operator=(Hash&& rhs);
    ~Hash();
    size_t update(const std::string& value);
    size_t update(const void* ptr, size_t size);
    size_t update(std::istream& stream);
    std::string getBase64();
  private:
    void release();

#if defined(USBGUARD_USE_LIBSODIUM)
    crypto_hash_sha256_state _state;
#endif
#if defined(USBGUARD_USE_OPENSSL)
    EVP_MD_CTX* _state;
#endif
#if defined(USBGUARD_USE_LIBGCRYPT)
    gcry_md_hd_t _state {nullptr};
#endif
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
