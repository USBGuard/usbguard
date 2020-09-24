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
#include "Rule.hpp"
#include "RuleSet.hpp"
#include "Typedefs.hpp"
#include "USB.hpp"

#include <map>
#include <string>
#include <vector>

#include <cstdint>

namespace usbguard
{

  /**
   * @brief Allows to receive signals and to communicate with
   * the USBGuard daemon.
   */
  class DLL_PUBLIC Interface
  {
  public:

    /*************************************************************************
     ******************************* PARAMETERS ******************************
     *************************************************************************/

    /**
     * @brief Set value of a runtime parameter.
     *
     * @param name Name of runtime parameter.
     * @param value Value of runtime parameter.
     * @return Previous value of runtime parameter.
     */
    virtual std::string setParameter(const std::string& name, const std::string& value) = 0;

    /**
     * @brief Get value of a runtime parameter.
     *
     * @param name Name of runtime parameter.
     * @return Value of runtime parameter.
     */
    virtual std::string getParameter(const std::string& name) = 0;

    /*************************************************************************
     ******************************* METHODS *********************************
     *************************************************************************/

    /**
     * @brief Append a new rule to the current policy.
     *
     * Using the \p parent_id parameter, the rule can be inserted anywhere in
     * the policy, not only at the end. When the rule is successfully appended,
     * the ID assigned to the new rule is returned.
     *
     * @param rule_spec The rule that should be appended to the policy.
     * @param parent_id Rule ID of the parent rule.
     * @param permanent Determines whether the rule will be added to
     * the policy file.
     * @return The rule ID assigned to the successfully appended rule.
     */
    virtual uint32_t appendRule(const std::string& rule_spec,
      uint32_t parent_id, bool permanent) = 0;

    /**
     * @brief Remove a rule from the current policy.
     *
     * @param id The rule ID of the rule to be removed.
     */
    virtual void removeRule(uint32_t id) = 0;

    /**
     * @brief List the current rule set (policy) used by the USBGuard daemon.
     *
     * The rules are returned in the same order as they are evaluated.
     *
     * @param query Query used for matching rules.
     * @return Vector of rules.
     */
    virtual const std::vector<Rule> listRules(const std::string& query) = 0;

    /**
     * @brief Apply an authorization target to a device.
     *
     * If the \p permanent flag is set to true, a rule will be appended to
     * the policy or an existing device rule will be modified in order to
     * permanently store the authorization decision.
     * Successful execution of this method will cause the DevicePolicyChanged
     * signal to be broadcasted if the device authorization target was
     * different then the applied target.
     *
     * @param id Device ID of the device to authorize.
     * @param target Device authorization target.
     * @param permanent Boolean flag specifying whether an allow rule should
     * be appended to the policy.
     * @return If \p permanent was set to true, the method will return an ID of
     * the rule that was modified or created because of this request.
     */
    virtual uint32_t applyDevicePolicy(uint32_t id,
      Rule::Target target,
      bool permanent) = 0;

    /**
     * @brief List devices that match the specified query.
     *
     * The query uses the rule language syntax and the devices are returned
     * as device specific rules. The target in each rule represents the current
     * authorization state of the device. Order of the returned devices is not
     * defined and should not be relied upon.
     *
     * @param query Query in the rule language syntax used to match devices.
     * @return Vector of device specific rules that match the query.
     */
    virtual const std::vector<Rule> listDevices(const std::string& query) = 0;

    /*************************************************************************
     ******************************* SIGNALS *********************************
     *************************************************************************/

    /**
     * @brief Notify that a USB device was already present when USBGuard
     * was started.
     *
     * The device attribute dictionary contains the following attributes:
     * - id (the USB device ID in the form VID:PID)
     * - name
     * - serial
     * - via-port
     * - hash
     * - parent-hash
     * - with-interface
     *
     * The USB interface types are represented as strings of the form AA:BB:CC,
     * where AA, BB and CC are hexadecimal numbers representing the class,
     * subclass and protocol of the interface as defined by the USB standard.
     *
     * @param id ID of the device.
     * @param event Type of the presence change event.
     * @param target The current authorization target of the device.
     * @param device_rule Device specific rule.
     */
    virtual void DevicePresenceChanged(uint32_t id,
      DeviceManager::EventType event,
      Rule::Target target,
      const std::string& device_rule) = 0;

    /**
     * @brief Notify about a change of a USB device authorization target.
     *
     * The device attribute dictionary contains the following attributes:
     * - id (the USB device ID in the form VID:PID)
     * - name
     * - serial
     * - via-port
     * - hash
     * - parent-hash
     * - with-interface
     *
     * @param id ID of the device.
     * @param target_old Previous authorization target.
     * @param target_new Current authorization target.
     * @param device_rule Device specific rule.
     * @param rule_id Rule ID of the matched rule.
     * Otherwise a reserved rule ID value is used.
     */
    virtual void DevicePolicyChanged(uint32_t id,
      Rule::Target target_old,
      Rule::Target target_new,
      const std::string& device_rule,
      uint32_t rule_id) = 0;

    /**
     * @brief Notify about the acceptance or rejection of a device.
     *
     * This signal is thrown whenever a device is inserted.
     * It is also thrown when a device has been allowed or rejected.
     *
     * The device attribute dictionary contains the following attributes:
     * - id (the USB device ID in the form VID:PID)
     * - name
     * - serial
     * - via-port
     * - hash
     * - parent-hash
     * - with-interface
     *
     * @param id ID of the device.
     * @param target_new Current authorization target.
     * @param device_rule Device specific rule.
     * @param rule_id Rule ID of the matched rule.
     * Otherwise a reserved rule ID value is used.
     */
    virtual void DevicePolicyApplied(uint32_t id,
      Rule::Target target_new,
      const std::string& device_rule,
      uint32_t rule_id) = 0;

    /**
     * @brief Notify about a change of a property parameter.
     *
     * @param name Policy name.
     * @param value_old Previous policy value.
     * @param value_new Current policy value.
     */
    virtual void PropertyParameterChanged(const std::string& name,
      const std::string& value_old,
      const std::string& value_new) = 0;

    /**
     * @brief Notify about an exception.
     *
     * @param context Description or identifier of the exception context.
     * @param object Description or identifier of the object which caused
     * the exception.
     * @param reason Reason explaining why the exception was generated.
     */
    virtual void ExceptionMessage(const std::string& context,
      const std::string& object,
      const std::string& reason) = 0;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
