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
#include "Hash.hpp"
#include "Base64.hpp"

namespace usbguard
{
  Hash::Hash()
  {
    /*
     * Initialize the hash state.
     *
     * TODO: Use a hash salt from the configuration.
     */
#if defined(USBGUARD_USE_LIBSODIUM)
    crypto_hash_sha256_init(&_state);
#endif
#if defined(USBGUARD_USE_LIBGCRYPT)
    gcry_md_open(&_state, GCRY_MD_SHA256, 0);
#endif
  }

  size_t Hash::update(const String& value)
  {
#if defined(USBGUARD_USE_LIBSODIUM)
    crypto_hash_sha256_update(&_state,
                              reinterpret_cast<const uint8_t *>(value.c_str()),
                              value.size());
#endif
#if defined(USBGUARD_USE_LIBGCRYPT)
    gcry_md_write(_state, value.c_str(), value.size());
#endif
    return value.size();
  }

  size_t Hash::update(std::istream& stream)
  {
    size_t size_hashed = 0;

    while (stream.good()) {
      uint8_t buffer[4096];
      size_t buflen = 0;

      stream.read(reinterpret_cast<char*>(buffer), sizeof buffer);
      buflen = stream.gcount();

      if (buflen > 0) {
#if defined(USBGUARD_USE_LIBSODIUM)
        crypto_hash_sha256_update(&_state, buffer, buflen);
#endif
#if defined(USBGUARD_USE_LIBGCRYPT)
        gcry_md_write(_state, buffer, buflen);
#endif
        size_hashed += buflen;
      }
    }
    return size_hashed;
  }

  String Hash::getBase64()
  {
#if defined(USBGUARD_USE_LIBSODIUM)
    uint8_t hash_binary[crypto_hash_sha256_BYTES];
    crypto_hash_sha256_final(&_state, hash_binary);
    const uint8_t * const hash_buffer = hash_binary;
    const size_t hash_buflen = sizeof hash_binary;
#endif
#if defined(USBGUARD_USE_LIBGCRYPT)
    gcry_md_final(_state);
    const size_t hash_buflen = gcry_md_get_algo_dlen(GCRY_MD_SHA256);
    const uint8_t * const hash_buffer = gcry_md_read(_state, GCRY_MD_SHA256);
#endif

    return base64Encode(hash_buffer, hash_buflen);
  }
} /* namespace usbguard */
