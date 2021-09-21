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

#include "DeviceManager.hpp"
#include "Interface.hpp"
#include "Rule.hpp"
#include "Typedefs.hpp"

#include <istream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <memory>

#include <cstddef>
#include <cstdint>

#include <unistd.h>
#include <sys/types.h>

namespace usbguard
{
  class IPCServerPrivate;

  /**
   * @brief Receives messages from \link IPCClient IPC clients\endlink\.
   * Provides USBGuard service.
   */
  class DLL_PUBLIC IPCServer : public Interface
  {
  public:

    /**
     * @brief Checks whether given name is a valid access control name.
     *
     * Name is a valid access control name iff:
     *  1. it is not longer then 32 characters
     *  2. it matches regex [A-Za-z_][A-Za-z0-9_-]*[$]
     *
     * @param name Name to be verified.
     * @throw Exception If \p name is not a valid access control name.
     */
    static void checkAccessControlName(const std::string& name);

    /**
     * @brief Limits access to the USBGuard IPC interface.
     *
     * Available sections, privileges and their meaning:
     *
     * -Devices
     *   -modify: change authorization state of devices including permanent
     *            changes (i.e. modification of device specific rules in the
     *            policy).
     *   -list  : Ability to get a list of recognized devices and their
     *            attributes.
     *   -listen: Listen to device presence and device policy changes.
     * -Policy
     *   -modify: Append rules to or remove any rules from the policy.
     *   -list  : Ability to view the currently enforced policy.
     * -Exceptions
     *   -listen: Receive exception messages.
     * -Parameters
     *   -modify: Set values of run-time parameters.
     *
     * There are also special sections and privileges \p NONE and \p ALL which
     * can be used to represent none or all of the sections or privileges
     * respectively.
     */
    class AccessControl
    {
    public:

      /**
       * @brief Possible sections for which privileges can be specified.
       */
      enum class Section : uint8_t {
        NONE = 0,
        DEVICES = 1,
        POLICY = 2,
        PARAMETERS = 3,
        EXCEPTIONS = 4,
        ALL = 255 /**< Represents all previous sections except for NONE. */
      };

      /**
       * @brief Returns \link Section section\endlink representation
       * of given string.
       *
       * @param section_string String to parse.
       * @return Section.
       * @throw runtime_error If given string is not a valid Section.
       */
      static Section sectionFromString(const std::string& section_string);

      /**
       * @brief Returns string representation of given \link Section
       * section\endlink.
       *
       * @param section Section to transform.
       * @return String representation of given Section.
       * @throw runtime_error If given \p section is not a valid Section.
       */
      static std::string sectionToString(const Section section);

      /**
       * @brief Possible privileges and their hexadecimal values.
       */
      enum class Privilege : uint8_t {
        NONE = 0x00, /**< Represents no priviliges. */
        LIST = 0x01,
        MODIFY = 0x02,
        LISTEN = 0x08,
        ALL = 0xff /**< Represents all previous privileges except for \p NONE. */
      };

      /**
       * @brief Returns \link Privilege privilege\endlink representation
       * of given string.
       *
       * @param privilege_string String to parse.
       * @return Privilege.
       * @throw runtime_error If given string is not a valid privilege.
       */
      static Privilege privilegeFromString(const std::string& privilege_string);

      /**
       * @brief Returns string representation of given \link Privilege
       * privilege\endlink.
       *
       * @param privilege Privilege to transform.
       * @return String representation of given Privilege.
       * @throw runtime_error If given privilege is not a valid Privilege.
       */
      static std::string privilegeToString(const Privilege privilege);

      /**
       * @brief Constructs empty \link AccessControl AccessControl\endlink
       * object containing no privileges.
       */
      AccessControl();

      /**
       * @brief Constructs \link AccessControl AccessControl\endlink
       * object by loading access control from given \p access_control_string.
       *
       * @param access_control_string String from which the access control
       * should be loaded.
       * @see \link load() load()\endlink
       */
      AccessControl(const std::string& access_control_string);

      /**
       * @brief Constructs \link AccessControl AccessControl\endlink
       * object with given privilege.
       *
       * @param section Section for which the privilege should be set.
       * @param privilege Privilege to be set.
       * @see setPrivilege()
       */
      AccessControl(Section section, Privilege privilege);

      /**
       * @brief Constructs \link AccessControl AccessControl\endlink
       * object that is a copy of rhs.
       *
       * @param rhs Object to copy.
       */
      AccessControl(const AccessControl& rhs);

      /**
       * @brief Assignes insides of object rhs to this.
       *
       * @param rhs Object to assign.
       * @return This.
       */
      AccessControl& operator=(const AccessControl& rhs);

      /**
       * @brief Tests whether access control contains given privilege.
       *
       * @param section Section for which the privilege should be tested.
       * @param privilege Privilege to test.
       * @return True if access control contains given privilege.
       * @throw USBGUARD_BUG If section is either \p ALL or \p NONE.
       */
      bool hasPrivilege(Section section, Privilege privilege) const;

      /**
       * @brief Inserts privilege into access control.
       *
       * @param section Section for which the privilege should be set.
       * @param privilege Privilede to be set.
       * @throw USBGUARD_BUG If section is \p NONE.
       */
      void setPrivilege(Section section, Privilege privilege);

      /**
       * @brief Clears access control.
       *
       * In other words, removes all priviledes for all sections from
       * access control and leaves it empty.
       */
      void clear();

      /**
       * @brief Loads access control from given stream.
       *
       * Every line in given input stream is parsed for presence
       * of tuple (Section, Privilege) which is then inserted into
       * access control using setPrivilege().
       *
       * @param stream Input stream to parse for access control.
       * @see \link setPrivilege() setPrivilege()\endlink
       */
      void load(std::istream& stream);

      /**
       * @brief Writes access control to given output stream.
       *
       * Access control is saved in following format:
       * \<section\>\=\<privilege1\>[,\<privilege2\>[,\<privilege3\>]]
       * where every section is on a separate line.
       *
       * @param stream Stream to which this access control should be saved.
       */
      void save(std::ostream& stream) const;

      /**
       * @brief Merges all privileges of rhs with this.
       *
       * @param rhs Object whose privileges will be merged with this.
       */
      void merge(const AccessControl& rhs);

      /**
       * @brief Merges all privileges contained in \p access_control_string
       * with this.
       *
       * @param access_control_string String containing privileges that
       * should be merged with this.
       */
      void merge(const std::string& access_control_string);

    private:

      /**
       * @brief Hash value for \link Section section\endlink.
       */
      struct SectionHash {

        /**
         * @brief Computes hash value for \link Section section\endlink.
         *
         * @param value Section.
         * @return Hash of given Section.
         */
        std::size_t operator()(Section value) const
        {
          return static_cast<std::size_t>(value);
        }
      };

      /**
       * @brief Get a privilege mask for given section
       *
       * For example, if the section is POLICY that has privileges MODIFY
       * and LIST, the mask would be ~(MODIFY | LIST)
       *
       * @param section Section for which the privilege mask should be returned
       * @return Privilege mask for section
       */
      uint8_t ac_mask(Section section) const;

      /**
       * @brief Access control represented by unordered map of
       * tuples (Section, 8b privileges).
       *
       * Individual privileges are united using bitwise OR.
       */
      std::unordered_map<Section, uint8_t, SectionHash> _access_control;
    };

    /**
     * @brief Initializes main loop, creates IPC server and USBGuard service.
     */
    IPCServer();

    /**
     * @brief Default destructor.
     */
    virtual ~IPCServer();

    /**
     * @brief Starts the main loop if it is not already running.
     */
    void start();

    /**
     * @brief Stops the main loop.
     */
    void stop();

    /**
     * @copydoc Interface::DevicePresenceChanged()
     */
    void DevicePresenceChanged(uint32_t id,
      DeviceManager::EventType event,
      Rule::Target target,
      const std::string& device_rule);

    /**
     * @copydoc Interface::DevicePolicyChanged()
     */
    void DevicePolicyChanged(uint32_t id,
      Rule::Target target_old,
      Rule::Target target_new,
      const std::string& device_rule,
      uint32_t rule_id);

    /**
     * @copydoc Interface::DevicePolicyApplied()
     */
    void DevicePolicyApplied(uint32_t id,
      Rule::Target target_new,
      const std::string& device_rule,
      uint32_t rule_id);

    /**
     * @copydoc Interface::PropertyParameterChanged()
     */
    void PropertyParameterChanged(const std::string& name,
      const std::string& value_old,
      const std::string& value_new);

    /**
     * @copydoc Interface::ExceptionMessage()
     */
    void ExceptionMessage(const std::string& context,
      const std::string& object,
      const std::string& reason);

    /**
     * @brief Adds user to a set of allowed users by specifying his user ID.
     *
     * @param uid User ID of user to allow.
     * @param ac Access control for given user.
     */
    void addAllowedUID(uid_t uid, const IPCServer::AccessControl& ac);

    /**
     * @brief Adds group to a set of allowed groups by specifying its group ID.
     *
     * @param gid Group ID of group to allow.
     * @param ac Access control for given group.
     */
    void addAllowedGID(gid_t gid, const IPCServer::AccessControl& ac);

    /**
     * @brief Adds user to a set of allowed users by specifying his user name.
     *
     * @param username User name of user to allow.
     * @param ac Access control for given user.
     */
    void addAllowedUsername(const std::string& username, const IPCServer::AccessControl& ac);

    /**
     * @brief Adds group to a set of allowed groups by specifying
     * its group name.
     *
     * @param groupname Group name of group to allow.
     * @param ac Access control for given group.
     */
    void addAllowedGroupname(const std::string& groupname, const IPCServer::AccessControl& ac);

  private:
    std::unique_ptr<IPCServerPrivate> d_pointer;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
