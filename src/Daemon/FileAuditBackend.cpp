//
// Copyright (C) 2017 Red Hat, Inc.
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

#include "FileAuditBackend.hpp"

#include <Common/Utility.hpp>

namespace usbguard
{
  void FileAuditBackend::write(const AuditEvent& event)
  {
    std::string message;
    message.append("uid=");
    message.append(numberToString(event.identity().uid()));
    message.append(" ");
    message.append("pid=");
    message.append(numberToString(event.identity().pid()));
    message.append(" ");

    for (const auto& kv_pair : event.keys()) {
      message.append(kv_pair.first);
      message.append("='");
      message.append(kv_pair.second);
      message.append("' ");
    }

    USBGUARD_LOG(Audit) << message;
  }
}

/* vim: set ts=2 sw=2 et */
