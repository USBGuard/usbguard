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

#include "IPCPrivate.hpp"

#include "Devices.pb.h"
#include "Exception.pb.h"
#include "Policy.pb.h"

#include "usbguard/Logger.hpp"

#include <vector>
#include <utility>

namespace usbguard
{
  static const std::vector<std::pair<uint32_t, std::string>> type_numbers = {
    { 0x01, "usbguard.IPC.listDevices" },
    { 0x02, "usbguard.IPC.applyDevicePolicy" },
    { 0x03, "usbguard.IPC.DevicePresenceChangedSignal" },
    { 0x04, "usbguard.IPC.DevicePolicyChangedSignal" },
    { 0x05, "usbguard.IPC.DevicePolicyAppliedSignal" },
    { 0x06, "usbguard.IPC.PropertyParameterChangedSignal" },
    { 0x07, "usbguard.IPC.listRules" },
    { 0x08, "usbguard.IPC.appendRule" },
    { 0x09, "usbguard.IPC.removeRule" },
    { 0x0a, "usbguard.IPC.Exception" },
    { 0x0b, "usbguard.IPC.getParameter" },
    { 0x0c, "usbguard.IPC.setParameter" },
    { 0x0d, "usbguard.IPC.checkIPCPermissions" }
  };

  uint32_t IPC::messageTypeNameToNumber(const std::string& name)
  {
    for (auto const& type_number : type_numbers) {
      if (type_number.second == name) {
        return type_number.first;
      }
    }

    throw std::runtime_error("Unknown IPC message type name");
  }

  const std::string& IPC::messageTypeNameFromNumber(const uint32_t number)
  {
    for (auto const& type_number : type_numbers) {
      if (type_number.first == number) {
        return type_number.second;
      }
    }

    throw std::runtime_error("Unknown IPC message type number");
  }

  IPC::MessagePointer IPC::IPCExceptionToMessage(const IPCException& exception)
  {
    IPC::Exception* const message = new IPC::Exception();
    IPC::MessagePointer pointer(message);
    message->set_context(exception.context());
    message->set_object(exception.object());
    message->set_reason(exception.reason());
    message->set_request_id(exception.messageID());
    return pointer;
  }

  IPCException IPC::IPCExceptionFromMessage(const MessagePointer& message)
  {
    const IPC::Exception* const exception_message = \
      reinterpret_cast<const IPC::Exception*>(message.get());
    return IPCException(exception_message->context(),
        exception_message->object(),
        exception_message->reason(),
        exception_message->request_id());
  }

  bool IPC::isExceptionMessage(const MessagePointer& message)
  {
    return message->GetTypeName() == IPC::Exception::default_instance().GetTypeName();
  }

  uint64_t IPC::getMessageHeaderID(const MessageType& message)
  {
    const auto header_field = message.GetDescriptor()->FindFieldByName("header");
    const auto reflection = message.GetReflection();
    const auto& header_message = reflection->GetMessage(message, header_field);

    if (header_message.GetTypeName() != IPC::MessageHeader::default_instance().GetTypeName()) {
      throw std::runtime_error("unknown message header type");
    }

    const auto header = reinterpret_cast<const IPC::MessageHeader&>(header_message);
    return header.id();
  }

  void IPC::setMessageHeaderID(MessageType& message, const uint64_t id)
  {
    const auto header_field = message.GetDescriptor()->FindFieldByName("header");
    auto reflection = message.GetReflection();
    auto header_message = reflection->MutableMessage(&message, header_field);

    if (header_message->GetTypeName() != IPC::MessageHeader::default_instance().GetTypeName()) {
      throw std::runtime_error("unknown message header type");
    }

    auto header = reinterpret_cast<IPC::MessageHeader*>(header_message);
    header->set_id(id);
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
