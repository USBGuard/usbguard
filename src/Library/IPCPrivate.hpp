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

#include "usbguard/Exception.hpp"
#include "usbguard/Typedefs.hpp"
#include "usbguard/IPCServer.hpp"

#include <memory>
#include <google/protobuf/message.h>

namespace usbguard
{
  namespace IPC
  {
    uint32_t messageTypeNameToNumber(const std::string& name);
    const std::string& messageTypeNameFromNumber(uint32_t number);

    using MessageType = google::protobuf::Message;
    using MessagePointer = std::unique_ptr<MessageType>;

    MessagePointer IPCExceptionToMessage(const IPCException& exception);
    IPCException IPCExceptionFromMessage(const MessagePointer& message);

    bool isExceptionMessage(const MessagePointer& message);

    uint64_t getMessageHeaderID(const MessageType& message);
    void setMessageHeaderID(MessageType& message, uint64_t id);

    template<class C>
    class MessageHandler
    {
    public:
      using HandlerType = void(C::*)(MessagePointer&, MessagePointer&);

      MessageHandler(C& c, HandlerType method, const MessageType& factory,
        IPCServer::AccessControl::Section section = IPCServer::AccessControl::Section::NONE,
        IPCServer::AccessControl::Privilege privilege = IPCServer::AccessControl::Privilege::NONE)
        : _instance(c),
          _method(method),
          _message_factory(factory),
          _section(section),
          _privilege(privilege)
      {
      }

      MessageHandler(const MessageHandler& rhs)
        : _instance(rhs._instance),
          _method(rhs._method),
          _message_factory(rhs._message_factory),
          _section(rhs._section),
          _privilege(rhs._privilege)
      {
      }

      MessagePointer payloadToMessage(const std::string& payload)
      {
        MessagePointer message(_message_factory.New());
        message->ParseFromString(payload);
        return message;
      }

      void run(MessagePointer& message)
      {
        MessagePointer response;
        run(message, response);
      }

      void run(MessagePointer& message, MessagePointer& response)
      {
        if (message->GetTypeName() != _message_factory.GetTypeName()) {
          throw std::runtime_error("Incompatible message type passed to handler");
        }

        (_instance.*_method)(message, response);
      }

      template<class ProtobufType>
      static MessageHandler create(C& c, HandlerType method,
        IPCServer::AccessControl::Section section = IPCServer::AccessControl::Section::NONE,
        IPCServer::AccessControl::Privilege privilege = IPCServer::AccessControl::Privilege::NONE)
      {
        return MessageHandler(c, method, ProtobufType::default_instance(), section, privilege);
      }

      IPCServer::AccessControl::Section section() const
      {
        return _section;
      }

      IPCServer::AccessControl::Privilege privilege() const
      {
        return _privilege;
      }

    private:
      C& _instance;
      HandlerType _method;
      const MessageType& _message_factory;
      IPCServer::AccessControl::Section _section;
      IPCServer::AccessControl::Privilege _privilege;
    };
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
