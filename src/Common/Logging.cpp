//
// Copyright (C) 2014 Red Hat, Inc.
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
#include <build-config.h>
#include "Common/Logging.hpp"
#include <vector>
#include <memory>

namespace usbguard
{
  std::shared_ptr<spdlog::logger> log;

  void setupLogger(bool debug, bool syslog, bool console, const String& file)
  {
    std::vector<spdlog::sink_ptr> sinks;

    if (console) {
      sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
    }
    if (!file.empty()) {
      sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_mt>(file));
    }
#ifdef __linux__
    if (syslog) {
      sinks.push_back(std::make_shared<spdlog::sinks::syslog_sink>("usbguardd"));
    }
#else
    (void)syslog;
#endif

    log = spdlog::create("logger", sinks.begin(), sinks.end());

    if (debug) {
      log->set_level(spdlog::level::trace);
    }

    return;
  }

  void destroyLogger()
  {
    log = nullptr;
    return;
  }
} /* namespace usbguard */
