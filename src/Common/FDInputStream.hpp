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

#include <ext/stdio_filebuf.h>
#include <fstream>
#include <memory>

namespace usbguard
{
#if HAVE_EXT_STDIO_FILEBUF_H
  class FDInputStream : public std::ifstream
  {
  public:
    FDInputStream(int fd)
      : _filebuf_ptr(new __gnu_cxx::stdio_filebuf<char>(fd, std::ios::in))
    {
      std::ios::rdbuf(_filebuf_ptr.get());
    }

    FDInputStream(FDInputStream&& stream)
      : _filebuf_ptr(std::move(stream._filebuf_ptr))
    {
      std::ios::rdbuf(_filebuf_ptr.get());
    }

  private:
    std::unique_ptr<__gnu_cxx::stdio_filebuf<char>> _filebuf_ptr;
  };
#else
#error "Required header file ext/stdio_filebuf.h not available."
#endif /* !HAVE_EXT_STDIO_FILEBUF_H */
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
