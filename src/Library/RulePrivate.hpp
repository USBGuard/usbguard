#pragma once
#include "Rule.hpp"

namespace usbguard {
  class RulePrivate
  {
  public:
    RulePrivate(Rule& p_instance);

    uint32_t getSeqn() const;
    const String& getVendorID() const;
    const String& getProductID() const;
    const String& getSerialNumber() const;
    const String& getDeviceName() const;
    const String& getDeviceHash() const;
    const StringVector& getDevicePorts() const;
    const StringVector& getInterfaceTypes() const;
    Rule::Target getTarget() const;
    const String& getAction() const;
    const std::chrono::steady_clock::time_point getTimePointAdded() const;
    uint32_t getTimeoutSeconds() const;

    bool appliesTo(Pointer<const Rule> rhs) const;
    bool appliesTo(const Rule& rhs) const;
    
    void setSeqn(uint32_t seqn);
    void setVendorID(const String& vendor_id);
    void setProductID(const String& product_id);
    void setSerialNumber(const String& serial_number);
    void setDeviceName(const String& device_name);
    void setDeviceHash(const String& device_hash);
    void setDevicePorts(const StringVector& device_ports);
    void setInterfaceTypes(const StringVector& interface_types);
    StringVector& refDevicePorts();
    StringVector& refInterfaceTypes();
    void setTarget(Rule::Target target);
    void setAction(const String& action);
    void setTimePointAdded(const std::chrono::steady_clock::time_point tp_added);
    void setTimeoutSeconds(uint32_t timeout_seconds);
    
    String toString() const;

    /*** Static methods ***/
    static Rule fromString(const String& rule_string);
    
  private:
    Rule& _p_instance;
    uint32_t _seqn;
    String _vendor_id;
    String _product_id;
    String _serial_number;
    String _device_name;
    String _device_hash;
    StringVector _device_ports;
    StringVector _interface_types;
    Rule::Target _target;
    String _action;
    std::chrono::steady_clock::time_point _tp_added;
    uint32_t _timeout_seconds;
  };
}
