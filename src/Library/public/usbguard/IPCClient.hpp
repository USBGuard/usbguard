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

#include "DeviceManager.hpp"
#include "Exception.hpp"
#include "Interface.hpp"
#include "IPCServer.hpp"
#include "Typedefs.hpp"

#include <string>
#include <vector>
#include <memory>

#include <cstdint>
#include <unistd.h>
#include <sys/types.h>


namespace usbguard
{
  class IPCClientPrivate;

  /**
   * @brief Communicates with USBGuard service
   * (\link IPCServer IPCServer\endlink).
   */
  class DLL_PUBLIC IPCClient : public Interface
  {
  public:

    /**
     * @brief Constructs IPC client.
     *
     * @param connected If this flag is set to true, then it tries to connect
     * to USBGuard \link IPCServer IPC service\endlink and starts IPC client
     * main loop.
     * @see \link connect() connect()\endlink
     */
    IPCClient(bool connected = false);

    /**
     * @brief Disconnects from an USBGuard \link IPCServer IPC service\endlink
     * and destructs this object.
     *
     * @see \link disconnect() disconnect()\endlink
     */
    virtual ~IPCClient();

    /**
     * @brief Creates connection to an USBGuard
     * \link IPCServer IPC service\endlink.
     *
     * Starts IPC client main loop.
     *
     * @throw ErrnoException If there is no service called \"usbguard\".
     * @throw Exception If we received bad file descriptor to poll.
     */
    void connect();

    /**
     * @brief Disconnects from an USBGuard \link IPCServer IPC service\endlink.
     *
     * Stops IPC client main loop.
     */
    void disconnect();

    /**
     * @brief Checks whether client is connected to USBGuard \link IPCServer
     * IPC service\endlink.
     *
     * @return True if this client is connected to USBGuard IPC service,
     * false otherwise.
     */
    bool isConnected() const;

    /**
     * @brief Wait for IPC client main loop to finish its work.
     */
    void wait();

    /**
     * @copydoc Interface::setParameter()
     */
    std::string setParameter(const std::string& name, const std::string& value) override;

    /**
     * @copydoc Interface::getParameter()
     */
    std::string getParameter(const std::string& name) override;

    /**
     * @copydoc Interface::appendRule()
     */
    uint32_t appendRule(const std::string& rule_spec, uint32_t parent_id, bool permanent) override;

    /**
     * @copydoc Interface::removeRule()
     */
    void removeRule(uint32_t id) override;

    /**
     * @copydoc Interface::listRules()
     */
    const std::vector<Rule> listRules(const std::string& label) override;

    /**
     * @brief List the current rule set (policy) used by the USBGuard daemon.
     *
     * The rules are returned in the same order as they are evaluated.
     *
     * @return Vector of rules.
     */
    const std::vector<Rule> listRules()
    {
      return listRules("");
    }

    /**
     * @copydoc Interface::applyDevicePolicy()
     */
    uint32_t applyDevicePolicy(uint32_t id, Rule::Target target, bool permanent) override;

    /**
     * @copydoc Interface::listDevices()
     */
    const std::vector<Rule> listDevices(const std::string& query) override;

    /**
     * @brief List all devices recognized by USBGuard daemon.
     *
     * @note Left for compatibility.
     *
     * @return Vector of device specific rules.
     */
    const std::vector<Rule> listDevices()
    {
      return listDevices("match");
    }

    /**
     * @brief Check if IPC client has enough permission for queried section with privilege.
     *
     * @param section Section to be checked for.
     * @param privilege Privilege to be checked for.
     *
     * @return True if IPC client has enough permission
     * for (section, privilege), otherwise false.
     */
    bool checkIPCPermissions(const IPCServer::AccessControl::Section& section,
      const IPCServer::AccessControl::Privilege& privilege);

    /**
     * @brief Defines algorithm to perform in the case of IPC connection.
     */
    virtual void IPCConnected() {}

    /**
     * @brief Defines algorithm to perform in the case of IPC disconnection.
     *
     * @param exception_initiated Determines whether disconnection was caused
     * by an exception.
     * @param exception Exception that caused the disconnection.
     * Exception is valid only if \p exception_initiated is set to true.
     */
    virtual void IPCDisconnected(bool exception_initiated, const IPCException& exception)
    {
      (void)exception_initiated;
      (void)exception;
    }

    /**
     * @brief Defines algorithm to perform in the case that USB device presence
     * has been changed.
     *
     * @see \link Interface::DevicePresenceChanged()
     * DevicePresenceChanged()\endlink
     */
    virtual void DevicePresenceChanged(uint32_t id,
      DeviceManager::EventType event,
      Rule::Target target,
      const std::string& device_rule) override
    {
      (void)id;
      (void)event;
      (void)target;
      (void)device_rule;
    }

    /**
     * @brief Defines algorithm to perform in the case that USB device
     * authorization target has been changed.
     *
     * @see \link Interface::DevicePolicyChanged()
     * DevicePolicyChanged()\endlink
     */
    virtual void DevicePolicyChanged(uint32_t id,
      Rule::Target target_old,
      Rule::Target target_new,
      const std::string& device_rule,
      uint32_t rule_id) override
    {
      (void)id;
      (void)target_old;
      (void)target_new;
      (void)device_rule;
      (void)rule_id;
    }

    /**
     * @brief Defines actions to perform when a USB device
     * has been inserted, accepted, or rejected.
     *
     * @see \link Interface::DevicePolicyApplied()
     * DevicePolicyApplied()\endlink
     */
    virtual void DevicePolicyApplied(uint32_t id,
      Rule::Target target_new,
      const std::string& device_rule,
      uint32_t rule_id) override
    {
      (void)id;
      (void)target_new;
      (void)device_rule;
      (void)rule_id;
    }

    /**
     * @brief Defines algorithm to perform in the case that property parameter
     * has been changed.
     *
     * @see \link Interface::PropertyParameterChanged()
     * PropertyParameterChanged()\endlink
     */
    virtual void PropertyParameterChanged(const std::string& name,
      const std::string& value_old,
      const std::string& value_new) override
    {
      (void)name;
      (void)value_old;
      (void)value_new;
    }

    /**
     * @brief Defines algorithm to perform in the case
     * that exception has arose.
     *
     * @see \link Interface::ExceptionMessage() ExceptionMessage()\endlink
     */
    virtual void ExceptionMessage(const std::string& context,
      const std::string& object,
      const std::string& reason) override
    {
      (void)context;
      (void)object;
      (void)reason;
    }

  private:
    std::unique_ptr<IPCClientPrivate> d_pointer;
  };

} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
