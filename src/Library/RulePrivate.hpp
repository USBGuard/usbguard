#pragma once
#include <build-config.h>
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
    const std::vector<USBInterfaceType>& getInterfaceTypes() const;
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
    void setInterfaceTypes(const std::vector<USBInterfaceType>& interface_types);
    StringVector& refDevicePorts();
    std::vector<USBInterfaceType>& refInterfaceTypes();
    void setDevicePortsSetOperator(Rule::SetOperator op);
    void setInterfaceTypesSetOperator(Rule::SetOperator op);
    void setTarget(Rule::Target target);
    void setAction(const String& action);
    void setTimePointAdded(const std::chrono::steady_clock::time_point tp_added);
    void setTimeoutSeconds(uint32_t timeout_seconds);
    
    String toString(bool invalid = false) const;

    /*** Static methods ***/
    static Rule fromString(const String& rule_string);
  protected:
    static void toString_addNonEmptyField(String& rule, const String& name, const String& value);
    static String quoteEscapeString(const String& value);

    /*
     * All of the items in rule set must match an item in the
     * applies_to set
     */
    template<typename T>
    static bool setSolveAllOf(const std::vector<T> rule_set, const std::vector<T>& applies_to_set)
    {
      for (auto const& rule_set_item : rule_set) {
	bool match = false;
	for (auto const& applies_to_set_item : applies_to_set) {
	  if (matches(rule_set_item, applies_to_set_item)) {
	    match = true;
	    break;
	  }
	}
	if (!match) {
	  return false;
	}
      }
      return true;
    }

    /*
     * At least one of the items in the rule set must match an item in the
     * applies_to set
     */
    template<typename T>
    static bool setSolveOneOf(const std::vector<T> rule_set, const std::vector<T>& applies_to_set)
    {
      for (auto const& rule_set_item : rule_set) {
	for (auto const& applies_to_set_item : applies_to_set) {
	  if (matches(rule_set_item, applies_to_set_item)) {
	    return true;
	  }
	}
      }
      return false;
    }

    /*
     * None of the the items in the rule set must match any item in the
     * applies_to set
     */
    template<typename T>
    static bool setSolveNoneOf(const std::vector<T> rule_set, const std::vector<T>& applies_to_set)
    {
      for (auto const& rule_set_item : rule_set) {
	for (auto const& applies_to_set_item : applies_to_set) {
	  if (matches(rule_set_item, applies_to_set_item)) {
	    return false;
	  }
	}
      }
      return true;
    }

    /*
     * Every item in the rule set must match one item in the
     * applies_to set and the sets have to have the same number
     * of items
     */
    template<typename T>
    static bool setSolveEquals(const std::vector<T> rule_set, const std::vector<T>& applies_to_set)
    {
      if (rule_set.size() != applies_to_set.size()) {
	return false;
      }
      else {
	for (auto const& rule_set_item : rule_set) {
	  bool match = false;
	  for (auto const& applies_to_set_item : applies_to_set) {
	    if (matches(rule_set_item, applies_to_set_item)) {
	      match = true;
	      break;
	    }
	  }
	  if (!match) {
	    return false;
	  }
	}
	return true;
      }
    }

    /*
     * The sets are treated as arrays and they have to me equal
     * (same number of items at the same positions)
     */
    template<typename T>
    static bool setSolveEqualsOrdered(const std::vector<T> rule_set, const std::vector<T>& applies_to_set)
    {
      if (rule_set.size() != applies_to_set.size()) {
	return false;
      }
      for (size_t i = 0; i < rule_set.size(); ++i) {
	if (!matches(rule_set[i], applies_to_set[i])) {
	  return false;
	}
      }
      return false;
    }

  private:
    Rule& _p_instance;
    uint32_t _seqn;
    String _vendor_id;
    String _product_id;
    String _serial_number;
    String _device_name;
    String _device_hash;
    StringVector _device_ports;
    Rule::SetOperator _device_ports_op;
    std::vector<USBInterfaceType> _interface_types;
    Rule::SetOperator _interface_types_op;
    Rule::Target _target;
    String _action;
    std::chrono::steady_clock::time_point _tp_added;
    uint32_t _timeout_seconds;
  };
}
