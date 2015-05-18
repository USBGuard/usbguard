//
// Copyright (C) 2015 Red Hat, Inc.
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

#include "Typedefs.hpp"
#include "Logger.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/null_sink.h>
#include <spdlog/sinks/file_sinks.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

namespace usbguard
{
  extern Pointer<spdlog::logger> logger;

  class LoggerPrivate
  {
  public:
    LoggerPrivate();
    ~LoggerPrivate();
    void create();

    void setConsoleOutput(bool state);
    void setSyslogOutput(bool state, const String& ident);
    void setFileOutput(bool state, const String& path);

  private:
    bool _console_enabled;
    bool _syslog_enabled;
    String _syslog_ident;
    bool _file_enabled;
    String _file_path;
    spdlog::level::level_enum _level;
    bool _created;
  };
} /* namespace usbguard */
