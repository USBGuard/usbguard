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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "Hash.hpp"
#include "Base64.hpp"
#include "Common/Utility.hpp"

#include "usbguard/Exception.hpp"

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
#if defined(USBGUARD_USE_OPENSSL)
#if OPENSSL_VERSION_NUMBER >= 0x10100000L

    if ((_state = EVP_MD_CTX_new()) == nullptr) {
      throw std::runtime_error("Dynamic memory allocation of message digest context failed.");
    }

#else

    if ((_state = EVP_MD_CTX_create()) == nullptr) {
      throw std::runtime_error("Dynamic memory allocation of message digest context failed.");
    }

#endif

    if (!EVP_DigestInit_ex(_state, EVP_sha256(), nullptr)) {
      throw std::runtime_error("Context initialization of message digest context failed.");
    }

#endif
#if defined(USBGUARD_USE_LIBGCRYPT)
    gcry_md_open(&_state, GCRY_MD_SHA256, 0);
#endif
  }

  Hash::Hash(const Hash& rhs)
  {
    release();
#if defined(USBGUARD_USE_LIBSODIUM)
    _state = rhs._state;
#endif
#if defined(USBGUARD_USE_OPENSSL)
    _state = rhs._state;
#endif
#if defined(USBGUARD_USE_LIBGCRYPT)
    gcry_md_copy(&_state, rhs._state);
#endif
  }

  Hash::Hash(Hash&& rhs)
  {
    release();
#if defined(USBGUARD_USE_LIBSODIUM)
    _state = rhs._state;
    memset(&rhs._state, 0, sizeof _state);
#endif
#if defined(USBGUARD_USE_OPENSSL)
    _state = rhs._state;
    memset(&rhs._state, 0, sizeof _state);
#endif
#if defined(USBGUARD_USE_LIBGCRYPT)
    _state = rhs._state;
    rhs._state = nullptr;
#endif
  }

  Hash& Hash::operator=(Hash&& rhs)
  {
    release();
#if defined(USBGUARD_USE_LIBSODIUM)
    _state = rhs._state;
    memset(&rhs._state, 0, sizeof _state);
#endif
#if defined(USBGUARD_USE_OPENSSL)
    _state = rhs._state;
    memset(&rhs._state, 0, sizeof _state);
#endif
#if defined(USBGUARD_USE_LIBGCRYPT)
    _state = rhs._state;
    rhs._state = nullptr;
#endif
    return *this;
  }

  Hash::~Hash()
  {
#if defined(USBGUARD_USE_OPENSSL)
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
    EVP_MD_CTX_free(_state);
#else
    EVP_MD_CTX_destroy(_state);
#endif
#endif
    release();
  }

  void Hash::release()
  {
#if defined(USBGUARD_USE_LIBSODIUM)
    memset(&_state, 0, sizeof _state);
#endif
#if defined(USBGUARD_USE_OPENSSL)
    memset(&_state, 0, sizeof _state);
#endif
#if defined(USBGUARD_USE_LIBGCRYPT)

    if (_state != nullptr) {
      gcry_md_close(_state);
    }

#endif
  }

  size_t Hash::update(const std::string& value)
  {
    return update(value.c_str(), value.size());
  }

  size_t Hash::update(const void* const ptr, const size_t size)
  {
#if defined(USBGUARD_USE_LIBSODIUM)
    crypto_hash_sha256_update(&_state, reinterpret_cast<const uint8_t*>(ptr), size);
#endif
#if defined(USBGUARD_USE_OPENSSL)

    if (!EVP_DigestUpdate(_state, reinterpret_cast<const uint8_t*>(ptr), size)) {
      throw std::runtime_error("Hashing data into message digest context failed.");
    }

#endif
#if defined(USBGUARD_USE_LIBGCRYPT)
    gcry_md_write(_state, ptr, size);
#endif
    return size;
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
#if defined(USBGUARD_USE_OPENSSL)

        if (!EVP_DigestUpdate(_state, buffer, buflen)) {
          throw std::runtime_error("Hashing data into message digest context failed.");
        }

#endif
#if defined(USBGUARD_USE_LIBGCRYPT)
        gcry_md_write(_state, buffer, buflen);
#endif
        size_hashed += buflen;
      }
    }

    return size_hashed;
  }

  std::string Hash::getBase64()
  {
#if defined(USBGUARD_USE_LIBSODIUM)
    uint8_t hash_binary[crypto_hash_sha256_BYTES];
    crypto_hash_sha256_final(&_state, hash_binary);
    const uint8_t* const hash_buffer = hash_binary;
    const size_t hash_buflen = sizeof hash_binary;
#endif
#if defined(USBGUARD_USE_OPENSSL)
    uint8_t hash_binary[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    if (!EVP_DigestFinal_ex(_state, hash_binary, &hash_len)) {
      throw std::runtime_error("Digest value retrieval failed.");
    }

    const uint8_t* const hash_buffer = hash_binary;
    const size_t hash_buflen = hash_len;
#endif
#if defined(USBGUARD_USE_LIBGCRYPT)
    gcry_md_final(_state);
    const size_t hash_buflen = gcry_md_get_algo_dlen(GCRY_MD_SHA256);
    const uint8_t* const hash_buffer = gcry_md_read(_state, GCRY_MD_SHA256);
#endif
    return base64Encode(hash_buffer, hash_buflen);
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
