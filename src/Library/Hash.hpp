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

#include <build-config.h>

#include "Typedefs.hpp"
#include <cstddef>
#include <istream>

#if defined(USBGUARD_USE_LIBSODIUM)
#include <sodium.h>
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
      size_t update(const String& value);
      size_t update(const void *ptr, size_t size);
      size_t update(std::istream& stream);
      String getBase64();
    private:
#if defined(USBGUARD_USE_LIBSODIUM)
      crypto_hash_sha256_state _state;
#endif
#if defined(USBGUARD_USE_LIBGCRYPT)
      gcry_md_hd_t _state;
#endif
  };
} /* namespace usbguard */
