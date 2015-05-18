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
#include <Typedefs.hpp>
#include "LoggerPrivate.hpp"
#include <vector>
#include <stdexcept>

namespace usbguard
{
  DLL_PUBLIC Pointer<spdlog::logger> logger = nullptr;
  static LoggerPrivate logger_state;

  void Logger::setVerbosityLevel(Level level)
  {
    spdlog::level::level_enum spdlog_level = spdlog::level::off;

    switch(level) {
    case Logger::Level::Trace:
      spdlog_level = spdlog::level::trace;
      break;
    case Logger::Level::Debug:
      spdlog_level = spdlog::level::debug;
      break;
    case Logger::Level::Info:
      spdlog_level = spdlog::level::info;
      break;
    case Logger::Level::Warning:
      spdlog_level = spdlog::level::warn;
      break;
    case Logger::Level::Error:
      spdlog_level = spdlog::level::err;
      break;
    }

    logger->set_level(spdlog_level);
    return;
  }

  void Logger::setConsoleOutput(bool state)
  {
    logger_state.setConsoleOutput(state);
    logger_state.create();
    return;
  }

  void Logger::setSyslogOutput(bool state, const String& ident)
  {
#if defined(__linux__)
    logger_state.setSyslogOutput(state, ident);
#else
    throw std::runtime_error("Syslog logger sink not supported");
#endif
    logger_state.create();
    return;
  }

  void Logger::setFileOutput(bool state, const String& path)
  {
    logger_state.setFileOutput(state, path);
    logger_state.create();
    return;
  }

  LoggerPrivate::LoggerPrivate()
  {
    _console_enabled = false;
    _syslog_enabled = false;
    _syslog_ident = "usbguard";
    _file_enabled = false;
    _file_path = "usbguard.log";
    _level = spdlog::level::off;
    _created = false;

    create();
  }

  LoggerPrivate::~LoggerPrivate()
  {
    logger = nullptr;
  }

  void LoggerPrivate::create()
  {
    std::vector<spdlog::sink_ptr> sinks;

    if (_console_enabled) {
      sinks.push_back(makePointer<spdlog::sinks::stdout_sink_mt>());
    }
    if (_file_enabled && !_file_path.empty()) {
      sinks.push_back(makePointer<spdlog::sinks::simple_file_sink_mt>(_file_path));
    }
#if defined(__linux__)
    if (_syslog_enabled) {
      sinks.push_back(makePointer<spdlog::sinks::syslog_sink>(_syslog_ident));
    }
#endif
    if (sinks.size() == 0) {
      sinks.push_back(makePointer<spdlog::sinks::null_sink_mt>());
    }

    if (_created) {
      spdlog::drop("usbguard");
      _created = false;
    }

    logger = spdlog::create("usbguard", sinks.begin(), sinks.end());
    logger->set_pattern("[%Y-%m-%d %T.%f] %l: %v");
    _created = true;

    return;
  }

  void LoggerPrivate::setConsoleOutput(bool state)
  {
    _console_enabled = state;
    return;
  }

  void LoggerPrivate::setSyslogOutput(bool state, const String& ident)
  {
    _syslog_enabled = state;
    _syslog_ident = ident;
    return;
  }

  void LoggerPrivate::setFileOutput(bool state, const String& path)
  {
    _file_enabled = state;
    _file_path = path;
    return;
  }
} /* namespace usbguard */
