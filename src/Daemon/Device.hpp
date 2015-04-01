#pragma once

#include "Typedefs.hpp"
#include "Rule.hpp"
#include "USB.hpp"
#include <mutex>

namespace usbguard {
  class Device
  {
  public:
    Device();

    std::mutex& refDeviceMutex();
    Pointer<Rule> getDeviceRule(bool include_port = false);
    uint32_t getSeqn() const;
    String getDeviceHash(bool include_port = false) const;
    void setSeqn(uint32_t seqn);
    void setTarget(Rule::Target target);
    void setDeviceName(const String& name);
    void setVendorID(const String& vendor_id);
    void setProductID(const String& product_id);
    void setSerialNumber(const String& serial_number);

    std::vector<USBInterfaceType>& refInterfaceTypes();

    void loadDeviceDescriptor(const USBDeviceDescriptor* descriptor);
    void loadConfigurationDescriptor(int c_num, const USBConfigurationDescriptor* descriptor);
    void loadInterfaceDescriptor(int c_num, int i_num, const USBInterfaceDescriptor* descriptor);

  private:
    std::mutex _mutex;
    uint32_t _seqn;
    Rule::Target _target;
    String _name;
    String _vendor_id;
    String _product_id;
    String _serial_number;
    std::vector<USBInterfaceType> _interface_types;
    int _num_configurations;
    int _num_interfaces;
  };
} /* namespace usbguard */
