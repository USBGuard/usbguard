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
#include "IPCPrivate.hpp"

namespace usbguard
{
  json IPCPrivate::IPCExceptionToJSON(const IPCException& ex)
  {
    json object = {
      { "_e", ex.codeAsString() },
      { "_i", ex.requestID() },
      { "message", ex.what() }
    };
    return object;
  }

  bool IPCPrivate::isExceptionJSON(const json& object)
  {
    return (object.count("_e") == 1);
  }

  IPCException IPCPrivate::jsonToIPCException(const json& object)
  {
    const std::string code_string = object["_e"];
    const IPCException::ReasonCode code = IPCException::codeFromString(code_string);

    if (object.find("message") != object.end()) {
      return IPCException(code, object["message"], object["_i"]);
    }
    else {
      return IPCException(code, "", object["_i"]);
    }
  }
} /* namespace usbguard */
