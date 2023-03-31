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

#include "Base64.hpp"
#include <stdexcept>

namespace usbguard
{
  static const char BASE64_PADDING_CHAR = '=';
  static const char encode_map[64+1] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

  static const char decode_map[80] = {
    /* + */ 62,
    /* , */ 0,
    /* - */ 0,
    /* . */ 0,
    /* / */ 63,
    /* 0 */ 52,
    /* 1 */ 53,
    /* 2 */ 54,
    /* 3 */ 55,
    /* 4 */ 56,
    /* 5 */ 57,
    /* 6 */ 58,
    /* 7 */ 59,
    /* 8 */ 60,
    /* 9 */ 61,
    /* : */ 0,
    /* ; */ 0,
    /* < */ 0,
    /* = */ 0,
    /* > */ 0,
    /* ? */ 0,
    /* @ */ 0,
    /* A */ 0,
    /* B */ 1,
    /* C */ 2,
    /* D */ 3,
    /* E */ 4,
    /* F */ 5,
    /* G */ 6,
    /* H */ 7,
    /* I */ 8,
    /* J */ 9,
    /* K */ 10,
    /* L */ 11,
    /* M */ 12,
    /* N */ 13,
    /* O */ 14,
    /* P */ 15,
    /* Q */ 16,
    /* R */ 17,
    /* S */ 18,
    /* T */ 19,
    /* U */ 20,
    /* V */ 21,
    /* W */ 22,
    /* X */ 23,
    /* Y */ 24,
    /* Z */ 25,
    /* [ */ 0,
    /* \ */ 0,
    /* ] */ 0,
    /* ^ */ 0,
    /* _ */ 0,
    /* ` */ 0,
    /* a */ 26,
    /* b */ 27,
    /* c */ 28,
    /* d */ 29,
    /* e */ 30,
    /* f */ 31,
    /* g */ 32,
    /* h */ 33,
    /* i */ 34,
    /* j */ 35,
    /* k */ 36,
    /* l */ 37,
    /* m */ 38,
    /* n */ 39,
    /* o */ 40,
    /* p */ 41,
    /* q */ 42,
    /* r */ 43,
    /* s */ 44,
    /* t */ 45,
    /* u */ 46,
    /* v */ 47,
    /* w */ 48,
    /* x */ 49,
    /* y */ 50,
    /* z */ 51
  };

#define B(n) (*(in+(n)))
  static void __b64_enc3(const uint8_t in[3], char out[4])
  {
    out[0] = encode_map[(B(0) & 0xfc) >> 2];
    out[1] = encode_map[(B(0) & 0x03) << 4 | (B(1) & 0xf0) >> 4];
    out[2] = encode_map[(B(1) & 0x0f) << 2 | (B(2) & 0xc0) >> 6];
    out[3] = encode_map[(B(2) & 0x3f)];
  }

  static void __b64_enc2 (const uint8_t in[2], char out[4])
  {
    out[0] = encode_map[(B(0) & 0xfc) >> 2];
    out[1] = encode_map[(B(0) & 0x03) << 4 | (B(1) & 0xf0) >> 4];
    out[2] = encode_map[(B(1) & 0x0f) << 2];
    out[3] = BASE64_PADDING_CHAR;
  }

  static void __b64_enc1 (const uint8_t in, char out[4])
  {
    out[0] = encode_map[(in & 0xfc) >> 2];
    out[1] = encode_map[(in & 0x03) << 4];
    out[2] = BASE64_PADDING_CHAR;
    out[3] = BASE64_PADDING_CHAR;
  }
#undef  B

  static void __check_b64_input(const char* in, const size_t count)
  {
    bool valid = true;

    for (size_t i = 0; i < count; ++i) {
      char c = in[i];

      if (c < '+' || c > 'z') {
        valid = false;
        break;
      }

      if (c > 'Z' && c < 'a') {
        valid = false;
        break;
      }

      if (c > '9' && c < 'A') {
        valid = false;
        break;
      }

      if (c > '+' && c < '/') {
        valid = false;
        break;
      }
    }

    if (!valid) {
      throw std::runtime_error("Invalid base64 input");
    }
  }

#define B(n) ((*(in+(n)) - '+') % (sizeof decode_map / sizeof(decode_map[0])))
  static void __b64_dec4 (const char in[4], uint8_t out[3])
  {
    __check_b64_input(in, 4);
    out[0] =   decode_map[B(0)]         << 2  | ((decode_map[B(1)] & 0x30) >> 4);
    out[1] = ((decode_map[B(1)] & 0x0f) << 4) | ((decode_map[B(2)] & 0x3c) >> 2);
    out[2] = ((decode_map[B(2)] & 0x03) << 6) |   decode_map[B(3)];
  }

  static void __b64_dec3 (const char in[3], uint8_t out[2])
  {
    __check_b64_input(in, 3);
    out[0] =   decode_map[B(0)]         << 2  | ((decode_map[B(1)] & 0x30) >> 4);
    out[1] = ((decode_map[B(1)] & 0x0f) << 4) | ((decode_map[B(2)] & 0x3c) >> 2);
  }

  static void __b64_dec2 (const char in[2], uint8_t out[1])
  {
    __check_b64_input(in, 2);
    out[0] = (decode_map[B(0)] << 2) | ((decode_map[B(1)] >> 4) & 0x03);
  }
#undef B

  std::string base64Encode (const uint8_t* const data, const size_t size)
  {
    if (size == 0 || data == nullptr) {
      throw std::runtime_error("base64encode: invalid input");
    }

    const size_t encoded_size = base64EncodedSize(size);
    const uint8_t remainder = size % 3;
    const size_t enc3_count = (size - remainder) / 3;
    std::string result(encoded_size, 0);
    char* const buffer = &result[0];
    size_t i = 0;

    for (; i < enc3_count; ++i) {
      __b64_enc3 (data + (i * 3), buffer + (i * 4));
    }

    switch (remainder) {
    case 2:
      __b64_enc2 (data + (i * 3), buffer + (i * 4));
      break;

    case 1:
      __b64_enc1 (*(data + (i * 3)), buffer + (i * 4));
      break;

    case 0:
      break;

    default:
      throw std::runtime_error("base64Encode: unexpected remainder value");
    }

    return result;
  }

  std::string base64Decode(const char* const data, const size_t size)
  {
    if (size == 0 || (size % 4) != 0) {
      throw std::runtime_error("base64Decode: invalid input");
    }

    const size_t decoded_size = base64DecodedSize(size);
    size_t dec4_count = size / 4;
    uint8_t padding = 0;

    if (data[size - 1] == BASE64_PADDING_CHAR) {
      if (data[size - 2] == BASE64_PADDING_CHAR) {
        padding = 2;
      }
      else {
        padding = 1;
      }

      --dec4_count;
    }

    std::string result(decoded_size, 0);
    uint8_t* const buffer = reinterpret_cast<uint8_t*>(&result[0]);
    size_t i = 0;

    for (; i < dec4_count; ++i) {
      __b64_dec4 (data + (i * 4), buffer + (i * 3));
    }

    switch (padding) {
    case 2:
      __b64_dec2 (data + (i * 4), buffer + (i * 3));
      result.resize(3 * i + 1);
      break;

    case 1:
      __b64_dec3 (data + (i * 4), buffer + (i * 3));
      result.resize(3 * i + 2);
      break;

    case 0:
      result.resize(3 * i);
      break;

    default:
      throw std::runtime_error("base64Device: unexpected padding value");
    }

    return result;
  }

  size_t base64EncodedSize(const size_t decoded_size)
  {
    return (decoded_size / 3 * 4) + ((decoded_size % 3) == 0 ? 0 : 4);
  }

  size_t base64DecodedSize(const size_t encoded_size)
  {
    return (encoded_size / 4 * 3) + (encoded_size % 4);
  }

  std::string base64Encode(const std::string& value)
  {
    return base64Encode(reinterpret_cast<const uint8_t*>(value.c_str()), value.size());
  }

  std::string base64Decode(const std::string& value)
  {
    return base64Decode(value.c_str(), value.size());
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
