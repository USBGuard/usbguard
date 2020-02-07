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
#pragma once

#include "Rule.hpp"
#include "Typedefs.hpp"
#include "USB.hpp"

#include <istream>
#include <mutex>
#include <string>
#include <memory>

#include <cstddef>
#include <cstdint>

namespace usbguard
{
  class DeviceManager;
  class DevicePrivate;

  /**
   * @brief Represents USB device in USBGuard.
   */
  class DLL_PUBLIC Device
  {
  public:

    /**
     * @brief Constructs new device object and sets its \link DeviceManager
     * device manager\endlink.
     *
     * Device ID is set to default, parent device ID is set to root ID and
     * target rule is set to unknown.
     *
     * @param manager \link DeviceManager device manager\endlink that is
     * responsible for this device.
     * @see \link Rule::DefaultID() DefaultID()\endlink
     * @see \link Rule::RootID() RootID()\endlink
     * @see \link Rule::Target Target\endlink
     */
    Device(DeviceManager& manager);

    /**
     * @brief Default destructor.
     */
    virtual ~Device();

    /**
     * @brief Constructs device object based on rhs.
     *
     * @param rhs Device object to copy.
     */
    Device(const Device& rhs);

    /**
     * @brief Assigns rhs to this.
     *
     * @param rhs Device object to assign.
     * @return This object.
     */
    const Device& operator=(const Device& rhs);

    /**
     * @brief Returns \link DeviceManager device manager\endlink that is
     * responsible for this device.
     *
     * @return \link DeviceManager Device manager\endlink that is responsible
     * for this device.
     */
    DeviceManager& manager() const;

    /**
     * @brief Returns reference to device mutex.
     *
     * @return Reference to device mutex.
     */
    std::mutex& refDeviceMutex();

    /**
     * @brief Generates device rule for this USB device.
     *
     * Generated device rule is composed of: ID, rule target, device ID,
     * serial number, connection type, port (optional), interface types,
     * device name, hash, parent hash (optional).
     *
     * @param with_port If true, port is included in generated rule.
     * @param with_parent_hash If true, parent hash is included in
     * generated rule.
     * @param match_rule If true, sets rule target to match.
     * @return Device rule for this USB device.
     * @throw runtime_error If \p with_parent_hash is true and parent hash
     * value is not available.
     */
    std::shared_ptr<Rule> getDeviceRule(bool with_port = true, bool with_parent_hash = true, bool match_rule = false);

    /**
     * @brief Hashes a given string and returns it.
     *
     * @param value Value to hash.
     * @return Hashed input string.
     */
    std::string hashString(const std::string& value) const;

    /**
     * @brief Initialize hash for this device.
     *
     * Hash is initialized from name, vendor ID, product ID and serial number
     * of the device.
     *
     * @throw Exception If vendor or product ID is empty.
     */
    void initializeHash();

    /**
     * @brief Hashes given data and creates new hash value.
     *
     * @param ptr Pointer to data to hash.
     * @param size Number of bytes to hash.
     */
    void updateHash(const void* ptr, size_t size);

    /**
     * @brief Hashes given descriptor stream and creates new hash value.
     *
     * @param descriptor_stream Descriptor stream to hash.
     * @param expected_size Expected number of bytes to hash.
     * @throw Exception If descriptor stream didnt return expected
     * amount of data.
     */
    void updateHash(std::istream& descriptor_stream, size_t expected_size);

    /**
     * @brief Finalizes hash by encoding it into base64.
     *
     * @return Finalized hash.
     */
    std::string finalizeHash();

    /**
     * @brief Returns device hash.
     *
     * @return Device hash.
     * @throw USBGUARD_BUG If device hash is not finalized.
     * @see \link finalizeHash() finalizeHash()\endlink
     */
    const std::string& getHash() const;

    /**
     * @brief Sets parent hash for this device.
     *
     * @param hash Parent hash to set.
     */
    void setParentHash(const std::string& hash);

    /**
     * @brief Sets device ID.
     *
     * @param id Device ID to set.
     */
    void setID(uint32_t id);

    /**
     * @brief Returns device ID.
     *
     * @return Device ID.
     */
    uint32_t getID() const;

    /**
     * @brief Sets parent ID for this device.
     *
     * @param id Parent ID to set.
     */
    void setParentID(uint32_t id);

    /**
     * @brief Returns parent ID.
     *
     * @return Parent ID.
     */
    uint32_t getParentID() const;

    /**
     * @brief Sets given target for this device.
     *
     * @param target Target to set.
     * @see \link Rule::Target Target\endlink
     */
    void setTarget(Rule::Target target);

    /**
     * @brief Returns rule target for this device.
     *
     * @return Rule target for this device.
     * @see \link Rule::Target Target\endlink
     */
    Rule::Target getTarget() const;

    /**
     * @brief Sets device name.
     *
     * @param name Device name to set.
     * @throw Exception If given \p name is bigger then
     * \p USB_GENERIC_STRING_MAX_LENGTH.
     */
    void setName(const std::string& name);

    /**
     * @brief Returns device name.
     *
     * @return Device name.
     */
    const std::string& getName() const;

    /**
     * @brief Sets device ID.
     *
     * @param device_id Device ID to set.
     */
    void setDeviceID(const USBDeviceID& device_id);

    /**
     * @brief Returns device ID.
     *
     * @return Device ID.
     */
    const USBDeviceID& getDeviceID() const;

    /**
     * @brief Sets port through which the device is connected.
     *
     * @param port Port to set.
     * @throw runtime_error If given \p port is bigger then
     * \p USB_PORT_STRING_MAX_LENGTH.
     */
    void setPort(const std::string& port);

    /**
     * @brief Returns port through which the device is connected.
     *
     * @return Port through which the device is connected.
     */
    const std::string& getPort() const;

    /**
     * @brief Sets device serial number.
     *
     * @param serial_number Serial number to set.
     * @throw runtime_error If given \p serial_number is bigger then
     * \p USB_GENERIC_STRING_MAX_LENGTH.
     */
    void setSerial(const std::string& serial_number);

    /**
     * @brief Returns device serial number.
     *
     * @return Device serial number.
     */
    const std::string& getSerial() const;

    /**
     * @brief Sets device connection type.
     *
     * @param connect_type Connection type to set.
     * @throw runtime_error If given \p connection_type is bigger then
     * \p USB_GENERIC_STRING_MAX_LENGTH.
     */
    void setConnectType(const std::string& connect_type);

    /**
     * @brief Returns device connection type.
     *
     * @return Device connection type.
     */
    const std::string& getConnectType() const;

    /**
     * @brief Returns reference to mutable vector of interface types
     * of this USB device.
     *
     * @return Reference to mutable vector of interface types
     * of this USB device.
     */
    std::vector<USBInterfaceType>& refMutableInterfaceTypes();

    /**
     * @brief Returns reference to vector of interface types of this USB device.
     *
     * @return Reference to vector of interface types of this USB device.
     */
    const std::vector<USBInterfaceType>& getInterfaceTypes() const;

    /**
     * @brief Checks whether this USB device is a controller or not.
     *
     * @return True if this USB device is a controller, false otherwise.
     */
    virtual bool isController() const = 0;

    /**
     * @brief Returns absolute path of this USB device.
     *
     * @return Absolute path of this USB device.
     */
    virtual std::string getSystemName() const = 0;

    /**
     * @brief Loads device descriptor using given USB descriptor parser.
     *
     * @param parser Parser that will parse the USB device descriptor.
     * @param descriptor USB Device descriptor.
     * @throw runtime_error If there are multiple device descriptors
     * for one device.
     */
    void loadDeviceDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor);

    /**
     * @brief Loads configuration descriptor using given USB descriptor parser.
     *
     * @param parser Parser that will parse the configuration descriptor.
     * @param descriptor Configuration descriptor.
     * @throw runtime_error If parent device descriptor is missing while
     * trying to load configuration descriptor.
     */
    void loadConfigurationDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor);

    /**
     * @brief Loads interface descriptor using given USB descriptor parser.
     *
     * @param parser Parser that will parse the interface descriptor.
     * @param descriptor Interface descriptor.
     * @throw runtime_error If parent configuration descriptor is missing while
     * trying to load interface descriptor.
     */
    void loadInterfaceDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor);

    /**
     * @brief Loads endpoint descriptor using given USB descriptor parser.
     *
     * @note Warning: This method can receive USB descriptors of two sizes!
     * (enpoint, audio endpoint).
     *
     * @param parser Parser that will parse the endpoint descriptor.
     * @param descriptor Endpoint descriptor.
     * @throw runtime_error If parent interface descriptor is missing while
     * trying to load enpoint descriptor.
     */
    void loadEndpointDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor);

  private:
    std::unique_ptr<DevicePrivate> d_pointer;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
