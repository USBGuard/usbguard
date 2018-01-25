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

#include "Common/Utility.hpp"

#include "usbguard/Logger.hpp"
#include "usbguard/Exception.hpp"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <syslog.h>
#ifndef __STDC_FORMAT_MACROS
  #define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>

namespace usbguard
{
  /* Instantiate the logger */
  Logger G_logger;

  const std::string LogStream::sourceToString(const Source& source)
  {
    return source.file + "@" + std::to_string(source.line) + "/" + source.function;
  }

  const std::string LogStream::levelToString(Level level)
  {
    switch (level) {
    case LogStream::Level::Audit:
      return "(A)";

    case LogStream::Level::Error:
      return "(E)";

    case LogStream::Level::Warning:
      return "(W)";

    case LogStream::Level::Info:
      return "(i)";

    case LogStream::Level::Debug:
      return "(D)";

    case LogStream::Level::Trace:
      return "(T)";

    default:
      throw std::runtime_error("BUG: unknown LogStream level value");
    }
  }

  LogStream::LogStream(Logger& logger, const Source& source, const Level level)
    : _logger(logger),
      _source(source),
      _level(level)
  {
  }

  LogStream::LogStream(const LogStream& rhs)
    : std::basic_ios<std::ostringstream::char_type, std::ostringstream::traits_type>(),
      std::ostringstream(rhs.str()),
      _logger(rhs._logger),
      _source(rhs._source),
      _level(rhs._level)
  {
  }

  LogStream::~LogStream()
  {
    _logger.write(_source, _level, str());
  }

  LogSink::LogSink(const std::string& name)
    : _name(name)
  {
  }

  LogSink::~LogSink()
  {
  }

  const std::string& LogSink::name() const
  {
    return _name;
  }

  /*
   * Internally required sinks
   */
  class OStreamSink : public LogSink
  {
  public:
    OStreamSink(const std::string& name, std::ostream& stream)
      : LogSink(name),
        _ostream(stream)
    {
    }

    void write(const LogStream::Source& source, LogStream::Level level, const std::string& message)
    {
      _ostream << '[' << Logger::timestamp() << "] ";
      _ostream << LogStream::levelToString(level) << " ";

      if (level >= LogStream::Level::Debug) {
        _ostream << LogStream::sourceToString(source) << ": ";
      }

      _ostream << message;
      _ostream << std::endl;
    }

    ~OStreamSink()
    {
      _ostream.flush();
    }
  private:
    std::ostream& _ostream;
  };

  class ConsoleSink : public OStreamSink
  {
  public:
    ConsoleSink()
      : OStreamSink("console", std::clog)
    {
    }
  };

  class SyslogSink : public LogSink
  {
  public:
    SyslogSink(const std::string& ident)
      : LogSink("syslog"),
        _ident(ident)
    {
      openlog(_ident.c_str(), LOG_NDELAY|LOG_PID|LOG_CONS, LOG_DAEMON);
    }

    ~SyslogSink()
    {
      closelog();
    }

    int levelToPriority(const LogStream::Level level)
    {
      switch (level) {
      case LogStream::Level::Audit:
        return LOG_NOTICE;

      case LogStream::Level::Error:
        return LOG_ERR;

      case LogStream::Level::Warning:
        return LOG_WARNING;

      case LogStream::Level::Info:
        return LOG_INFO;

      case LogStream::Level::Debug:
      case LogStream::Level::Trace:
        return LOG_DEBUG;

      default:
        throw USBGUARD_BUG("Invalid LogStream::Level value");
      }
    }

    void write(const LogStream::Source& source, LogStream::Level level, const std::string& message)
    {
      std::string log_message;

      if (level >= LogStream::Level::Debug) {
        log_message.append(LogStream::sourceToString(source));
        log_message.append(": ");
      }

      log_message.append(message);
      syslog(levelToPriority(level), "%s", log_message.c_str());
    }

  private:
    std::string _ident;
  };

  class FileSink : public OStreamSink
  {
  public:
    FileSink(const std::string& filepath, bool append = true)
      : OStreamSink("file", _stream)
    {
      _filepath = filepath;

      try {
        _stream.exceptions(std::fstream::failbit);
        _stream.open(filepath, append ? std::fstream::app : std::fstream::trunc);
      }
      catch (...) {
        throw Exception("FileSink", filepath, "failed to open");
      }
    }

    ~FileSink()
    {
      _stream.close();
    }
  private:
    std::string _filepath;
    std::ofstream _stream;
  };

  class AuditFileSink : public OStreamSink
  {
  public:
    AuditFileSink(const std::string& filepath)
      : OStreamSink("auditfile", _stream)
    {
      _filepath = filepath;
      const auto saved_umask = umask(0177);

      try {
        _stream.exceptions(std::fstream::failbit);
        _stream.open(filepath, std::fstream::app);
      }
      catch (...) {
        umask(saved_umask);
        throw Exception("AuditFileSink", filepath, "failed to open");
      }

      umask(saved_umask);
    }

    void write(const LogStream::Source& source, LogStream::Level level, const std::string& message)
    {
      /*
       * AuditFileSink logs only Audit level messages.
       */
      if (level == LogStream::Level::Audit) {
        OStreamSink::write(source, level, message);
      }
    }

    ~AuditFileSink()
    {
      _stream.close();
    }
  private:
    std::string _filepath;
    std::ofstream _stream;
  };

  Logger::Logger()
    : _enabled(true),
      _level(LogStream::Level::Warning)
  {
    const char* const envval = getenv("USBGUARD_DEBUG");

    /*
     * If USBGUARD_DEBUG=1 is set in the current environment,
     * set the debugging level to the highest level.
     */
    if (envval != nullptr && strcmp(envval, "1") == 0) {
      _level = LogStream::Level::Trace;
    }

    setOutputConsole(true);
  }

  Logger::~Logger()
  {
  }

  std::unique_lock<std::mutex> Logger::lock() const
  {
    return std::unique_lock<std::mutex>(_mutex);
  }

  void Logger::setEnabled(bool state, LogStream::Level level)
  {
    auto L = lock();
    _enabled = state;
    _level = level;
  }

  bool Logger::isEnabled(LogStream::Level level) const
  {
    auto L = lock();
    return (_enabled && _level >= level);
  }

  void Logger::setOutputConsole(const bool state)
  {
    auto L = lock();

    if (state == true) {
      std::unique_ptr<LogSink> sink(new ConsoleSink);
      addOutputSink_nolock(sink);
    }
    else {
      delOutputSink_nolock("console");
    }
  }

  void Logger::setOutputFile(bool state, const std::string& filepath, bool append)
  {
    auto L = lock();

    if (state == true) {
      std::unique_ptr<LogSink> sink(new FileSink(filepath, append));
      addOutputSink_nolock(sink);
    }
    else {
      delOutputSink_nolock("file");
    }
  }

  void Logger::setOutputSyslog(bool state, const std::string& ident)
  {
    auto L = lock();

    if (state == true) {
      std::unique_ptr<LogSink> sink(new SyslogSink(ident));
      addOutputSink_nolock(sink);
    }
    else {
      delOutputSink_nolock("syslog");
    }
  }

  void Logger::setAuditFile(bool state, const std::string& filepath)
  {
    auto L = lock();

    if (state == true) {
      std::unique_ptr<LogSink> sink(new AuditFileSink(filepath));
      addOutputSink_nolock(sink);
    }
    else {
      delOutputSink_nolock("auditfile");
    }
  }

  void Logger::addOutputSink(std::unique_ptr<LogSink>& sink)
  {
    auto L = lock();
    addOutputSink_nolock(sink);
  }

  void Logger::addOutputSink_nolock(std::unique_ptr<LogSink>& sink)
  {
    _sinks.emplace(sink->name(), std::move(sink));
  }

  void Logger::delOutputSink(const std::string& name)
  {
    auto L = lock();
    delOutputSink_nolock(name);
  }

  void Logger::delOutputSink_nolock(const std::string& name)
  {
    _sinks.erase(name);
  }

  LogStream Logger::operator()(const std::string& file, const int line, const std::string& function, LogStream::Level level)
  {
    const LogStream::Source source = {
      filenameFromPath(file, /*include_extension=*/true), line, function
    };
    return LogStream(*this, source, level);
  }

  void Logger::write(const LogStream::Source& source, const LogStream::Level level, const std::string& message)
  {
    auto L = lock();

    for (auto& kv_pair : _sinks) {
      auto& sink = kv_pair.second;

      try {
        sink->write(source, level, message);
      }
      catch (const std::exception& ex) {
        std::cerr << "Warning: sink->write failed for " << sink->name() << " sink: " << ex.what() << std::endl;
      }
    }
  }

  /*
   * Generate a timestamp string in the form:
   * <seconds>.<microseconds>
   */
  const std::string Logger::timestamp()
  {
    struct timeval tv_now = { 0, 0 };

    if (gettimeofday(&tv_now, nullptr) != 0) {
      throw std::runtime_error("gettimeofday");
    }

    /*
     * The following piece of code should work fine until
     * Sat Nov 20 17:46:39 UTC 2286.
     */
    char buffer[16];
    const int length = snprintf(buffer, sizeof buffer, "%.10" PRIu64 ".%03" PRIu64,
        (uint64_t)tv_now.tv_sec,
        (uint64_t)(tv_now.tv_usec / 1000));

    if (length < 1 || static_cast<size_t>(length) > (sizeof buffer - 1)) {
      throw std::runtime_error("Failed to convert timestamp to string");
    }

    return std::string(buffer, (size_t)length);
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
