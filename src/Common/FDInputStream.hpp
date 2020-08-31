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

#if HAVE_EXT_STDIO_FILEBUF_H
  #include <ext/stdio_filebuf.h>
#else
  #include <cstdio>
  #include <streambuf>
#endif /* !HAVE_EXT_STDIO_FILEBUF_H */
#include <fstream>
#include <memory>
#include <unistd.h>

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
  class FDStreamBuf : public std::streambuf
  {
  public:
    FDStreamBuf(int fd) : fd_(fd) { }

    std::streamsize xsgetn(char* s, std::streamsize n)
    {
      ssize_t ret;

      do {
        ret = read(fd_, s, n);
      }
      while (ret == -1 && (errno == EAGAIN || errno ==EINTR));

      if (ret < 0) {
        return EOF;
      }

      return ret;
    }

    int underflow()
    {
      char c;
      ssize_t result = xsgetn(&c, 1);

      if (result != 1) {
        return EOF;
      }

      return c;
    }
  private:
    int fd_;
  };

  class FDInputStream : public std::istream
  {
  public:
    FDInputStream(int fd) : std::istream(nullptr),
      _filebuf_ptr(new FDStreamBuf(fd))
    {
      std::ios::rdbuf(_filebuf_ptr.get());
    }

    FDInputStream(FDInputStream&& stream) : std::istream(nullptr),
      _filebuf_ptr(std::move(stream._filebuf_ptr))
    {
      std::ios::rdbuf(_filebuf_ptr.get());
    }

  private:
    std::unique_ptr<FDStreamBuf> _filebuf_ptr;
  };
#endif /* !HAVE_EXT_STDIO_FILEBUF_H */
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
