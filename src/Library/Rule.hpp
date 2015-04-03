#pragma once

#include <Typedefs.hpp>
#include <USB.hpp>
#include <cstdint>
#include <chrono>

namespace usbguard {
  class RulePrivate;
  class Rule
  {
  public:
    /**
     * Rule target enumeration.
     */
    enum Target {
      Allow, /**< Devices matching this rule will be authorized */
      Block, /**< Devices matching this rule will not be authorized */
      Reject, /**< Devices matching this rule will not be authorized and will be detached */
      Match, /**< Special target can be used to trigger actions. The rule wont affect the final decision. */
      Unknown, /**< Unknown target. Used for default constructed rules. */
      Invalid
    };

    /**< Sequence number of the (fake) root rule */
    static const uint32_t SeqnRoot = std::numeric_limits<uint32_t>::min();
    /**< Sequence number assigned to default constructed rules. Cannot be used for searching. */
    static const uint32_t SeqnDefault = std::numeric_limits<uint32_t>::max();
    /**< Sequence number for specifying that the last rule in the ruleset should be used in context of the operation */
    static const uint32_t SeqnLast = std::numeric_limits<uint32_t>::max() - 1;

    enum class SetOperator {
      AllOf,
      OneOf,
      NoneOf,
      Equals,
      EqualsOrdered,
      Match /* Special operator: matches anything, cannot be used directly in a rule */
    };

    /**
     * Construct a default rule.
     * This rule matches only a default rule and cannot be converted to a string
     * representation.
     */
    Rule();
    ~Rule();
    Rule(const Rule& rhs);

    uint32_t getSeqn() const;
    const String& getVendorID() const;
    const String& getProductID() const;
    const String& getSerialNumber() const;
    const String& getDeviceName() const;
    const String& getDeviceHash() const;
    const StringVector& getDevicePorts() const;
    const std::vector<USBInterfaceType>& getInterfaceTypes() const;
    Target getTarget() const;
    const String& getAction() const;
    const std::chrono::steady_clock::time_point getTimePointAdded() const;
    uint32_t getTimeoutSeconds() const;

    bool appliesTo(Pointer<const Rule> rhs) const;
    bool appliesTo(const Rule& rhs) const;
    bool isImplicit() const;
    
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

    operator bool() const;
    String toString(bool invalid = false) const;

    /*** Static methods ***/
    static Rule fromString(const String& rule_string);
    static SetOperator setOperatorFromString(const String& set_operator_string);
    static const String setOperatorToString(SetOperator op);
    static Target targetFromString(const String& target_string);
    static const String targetToString(Target target);

  private:
    RulePrivate* d_pointer;
  };
}
