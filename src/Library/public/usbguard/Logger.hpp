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

#include "Typedefs.hpp"

#include <fstream>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>

namespace usbguard
{
  class Logger;

  class DLL_PUBLIC LogStream : public std::ostringstream
  {
  public:
    struct Source {
      std::string file;
      int line;
      std::string function;
    };

    static const std::string sourceToString(const Source& source);

    enum class Level : int {
      Audit = -2,
      Error = -1,
      Warning = 0,
      Info = 1,
      Debug = 2,
      Trace = 3
    };

    static const std::string levelToString(Level level);

    LogStream(Logger& logger, const Source& source, Level level);
    LogStream(const LogStream& rhs);

    ~LogStream();

  private:
    Logger& _logger;
    Source _source;
    Level _level;
  };

  class DLL_PUBLIC LogSink
  {
  public:
    LogSink(const std::string& name);
    virtual ~LogSink();

    const std::string& name() const;

    virtual void write(const LogStream::Source& source, LogStream::Level level, const std::string& message) = 0;

  private:
    std::string _name;
  };

  class DLL_PUBLIC Logger
  {
  public:
    Logger();
    ~Logger();

    void setEnabled(bool state, LogStream::Level level = LogStream::Level::Warning);
    bool isEnabled(LogStream::Level level) const;

    void setOutputConsole(bool state);
    void setOutputFile(bool state, const std::string& filepath = std::string(), bool append = true);
    void setOutputSyslog(bool state, const std::string& ident = std::string());
    void setAuditFile(bool state, const std::string& filepath);

    void addOutputSink(std::unique_ptr<LogSink>& sink);
    void delOutputSink(const std::string& name);

    LogStream operator()(const std::string& file, int line, const std::string& function, LogStream::Level level);

    void write(const LogStream::Source& source, LogStream::Level level, const std::string& message);

    /*
     * Generate a timestamp string in the form:
     * <seconds>.<microseconds>
     */
    static const std::string timestamp();

  private:
    void addOutputSink_nolock(std::unique_ptr<LogSink>& sink);
    void delOutputSink_nolock(const std::string& name);

    std::unique_lock<std::mutex> lock() const;

    mutable std::mutex _mutex;
    bool _enabled;
    LogStream::Level _level;
    std::map<std::string, std::unique_ptr<LogSink>> _sinks;
  };

  extern DLL_PUBLIC Logger G_logger;

#if defined(__GNUC__)
  #define USBGUARD_SOURCE_FILE __BASE_FILE__
#else
  #define USBGUARD_SOURCE_FILE __FILE__
#endif

#define USBGUARD_LOGGER usbguard::G_logger

#define USBGUARD_FUNCTION __func__

#define USBGUARD_LOG(level) \
  if (USBGUARD_LOGGER.isEnabled(usbguard::LogStream::Level::level)) \
    USBGUARD_LOGGER(USBGUARD_SOURCE_FILE, __LINE__, USBGUARD_FUNCTION, usbguard::LogStream::Level::level)

} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
