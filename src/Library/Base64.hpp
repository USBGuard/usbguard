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

#include <string>
#include <cstdint>
#include <cstddef>

namespace usbguard
{
  size_t base64EncodedSize(size_t decoded_size);
  size_t base64DecodedSize(size_t encoded_size);

  std::string base64Encode(const std::string& value);
  std::string base64Encode(const uint8_t* buffer, size_t buflen);

  std::string base64Decode(const std::string& value);
  size_t base64Decode(const std::string& value, void* buffer, size_t buflen);
  std::string base64Decode(const char* const data, const size_t size);
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
