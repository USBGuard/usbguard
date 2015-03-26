#pragma once

#include "Typedefs.hpp"
#include "Rule.hpp"
#include <mutex>

namespace usbguard {
  class Device
  {
  public:
    Device();

    std::mutex& refDeviceMutex();
    Pointer<Rule> getDeviceRule();
    uint32_t getSeqn() const;
    void setSeqn(uint32_t seqn);
    void setTarget(Rule::Target target);
    void setDeviceName(const String& name);
    void setDeviceHash(const String& hash);
    void setVendorID(const String& vendor_id);
    void setProductID(const String& product_id);
    void setSerialNumber(const String& serial_number);
    void setInterfaceTypes(const StringVector& types);
    StringVector& refInterfaceTypes();

  private:
    std::mutex _mutex;
    uint32_t _seqn;
    Rule::Target _target;
    String _name;
    String _hash;
    String _vendor_id;
    String _product_id;
    String _serial_number;
    StringVector _interface_types;
  };
} /* namespace usbguard */
