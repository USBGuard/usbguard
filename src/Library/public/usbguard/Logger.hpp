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

  /**
   * @brief Log stream.
   */
  class DLL_PUBLIC LogStream : public std::ostringstream
  {
  public:

    /**
     * @brief Contains information about the source.
     */
    struct Source {
      std::string file; /**< File name. */
      int line; /**< Line number. */
      std::string function; /**< Name of the function. */
    };

    /**
     * @brief Returns string representation of given
     * \link Source source\endlink.
     *
     * Format: \<file\>\@\<line\>/\<function\>
     *
     * @param source Source to transform into string.
     * @return String representation of given \link Source source\endlink.
     * @see \link Source Source\endlink
     */
    static const std::string sourceToString(const Source& source);

    /**
     * @brief Log levels.
     */
    enum class Level : int {
      Audit = -2,
      Error = -1,
      Warning = 0,
      Info = 1,
      Debug = 2,
      Trace = 3
    };

    /**
     * @brief Returns string representation of given \link Level level\endlink.
     *
     * Example: levelToString(Audit) = (A)
     *
     * @param level Logger level to transform into string.
     * @return String representation of given \link Level level\endlink.
     * @throw runtime_error If \p level is invalid.
     * @see \link Level level\endlink
     */
    static const std::string levelToString(Level level);

    /**
     * @brief Constructs new LogStream with given \link Logger logger\endlink,
     * \link Source source\link and \link Level level\endlink.
     *
     * @param logger \link Logger Logger\endlink.
     * @param source \link Source Source\endlink.
     * @param level \link Level Log level\endlink.
     */
    LogStream(Logger& logger, const Source& source, Level level);

    /**
     * @brief Constructs copy of LogStream rhs.
     *
     * @param rhs LogStream to copy.
     */
    LogStream(const LogStream& rhs);

    /**
     * @brief Writes into a log before destruction.
     */
    ~LogStream();

  private:
    Logger& _logger;
    Source _source;
    Level _level;
  };

  /**
   * @brief Logger output sink.
   */
  class DLL_PUBLIC LogSink
  {
  public:

    /**
     * @brief Constructs logger output sink with given name.
     *
     * @param name Name of the sink.
     */
    LogSink(const std::string& name);

    /**
     * @brief Default destructor.
     */
    virtual ~LogSink();

    /**
     * @brief Returns name of the sink.
     *
     * @return Name of the sink.
     */
    const std::string& name() const;

    /**
     * @brief Writes given information into the sink.
     *
     * @param source \link Source Source\endlink.
     * @param level \link Level Log level\endlink.
     * @param message \link Message message\endlink.
     */
    virtual void write(const LogStream::Source& source, LogStream::Level level, const std::string& message) = 0;

  private:
    std::string _name;
  };

  /**
   * @brief Logger.
   */
  class DLL_PUBLIC Logger
  {
  public:

    /**
     * @brief Constructs logger with enabled log level set to warning.
     */
    Logger();

    /**
     * @brief Default destructor.
     */
    ~Logger();

    /**
     * @brief Enables/disables given level
     *
     * @note Default level is warning.
     *
     * @param state Determines if given level should be enabled/disabled.
     * @param level Log level to enable/disable.
     */
    void setEnabled(bool state, LogStream::Level level = LogStream::Level::Warning);

    /**
     * @brief Checks if given level is enabled.
     *
     * Internally checks whether \p current_level \>= \p given_level.
     *
     * @param level \link Level Log level\endlink to check.
     * @return True if given level is enabled, false otherwise.
     */
    bool isEnabled(LogStream::Level level) const;

    /**
     * @brief Adds/deletes console output sink to/from logger sinks.
     *
     * @param state If true, then console output sink is added to logger
     * output sinks, otherwise it is deleted from sinks.
     */
    void setOutputConsole(bool state);

    /**
     * @brief Adds/deletes file output sink to/from logger sinks.
     *
     * @param state If true, then file output sink is added to logger
     * output sinks, otherwise it is deleted from sinks.
     * @param append Opens file output sink in append mode.
     */
    void setOutputFile(bool state, const std::string& filepath = std::string(), bool append = true);

    /**
     * @brief Adds/deletes syslog output sink to/from logger sinks.
     *
     * @param state If true, then syslog output sink is added to logger
     * output sinks, otherwise it is deleted from sinks.
     */
    void setOutputSyslog(bool state, const std::string& ident = std::string());

    /**
     * @brief Adds/deletes audit file output sink to/from logger sinks.
     *
     * @param state If true, then audit file output sink is added to logger
     * output sinks, otherwise it is deleted from sinks.
     */
    void setAuditFile(bool state, const std::string& filepath);

    /**
     * @brief Adds given sink into logger output sinks.
     *
     * @param sink Output sink to add.
     */
    void addOutputSink(std::unique_ptr<LogSink>& sink);

    /**
     * @brief Deletes given sink from logger output sinks.
     *
     * @param name Name of output sink to delete.
     */
    void delOutputSink(const std::string& name);

    /**
     * @brief Constructs and returns new \link LogStream LogStream\endlink
     * object initialized with this logger, source containing given file,
     * line and function, and given level.
     *
     * @param file File name.
     * @param line Line number.
     * @param function Name of the function.
     * @param level \link Level Log level\endlink.
     * @return New \link LogStream LogStream\endlink object.
     */
    LogStream operator()(const std::string& file, int line, const std::string& function, LogStream::Level level);

    /**
     * @brief Writes information into all sinks.
     *
     * @param source \link Source Source\endlink.
     * @param level \link Level Log level\endlink.
     * @param message \link Message Message\endlink.
     */
    void write(const LogStream::Source& source, LogStream::Level level, const std::string& message);

    /**
     * @brief Generate a timestamp string.
     *
     * Format: \<seconds\>\.\<microseconds\>
     *
     * @return Timestamp string.
     */
    static const std::string timestamp();

  private:

    /**
     * @brief Adds given sink into logger output sinks without using a lock.
     *
     * @param sink Output sink to add.
     */
    void addOutputSink_nolock(std::unique_ptr<LogSink>& sink);

    /**
     * @brief Deletes given sink from logger output sinks without using a lock.
     *
     * @param name Name of output sink to delete.
     */
    void delOutputSink_nolock(const std::string& name);

    /**
     * @brief Return loggers lock.
     *
     * @return Loggers lock.
     */
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
