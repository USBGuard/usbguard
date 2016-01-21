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
#include "RulePrivate.hpp"
#include "Common/Utility.hpp"
#include <utility>
#include <locale>

namespace usbguard {
  const uint32_t Rule::SeqnRoot = std::numeric_limits<uint32_t>::min();
  const uint32_t Rule::SeqnDefault = std::numeric_limits<uint32_t>::max();
  const uint32_t Rule::SeqnLast = std::numeric_limits<uint32_t>::max() - 1;

  Rule::Rule()
  {
    d_pointer = new RulePrivate(*this);
  }

  Rule::~Rule()
  {
    delete d_pointer;
    d_pointer = nullptr;
  }

  Rule::Rule(const Rule& rhs)
  {
    d_pointer = new RulePrivate(*this, *rhs.d_pointer);
    return;
  }

  const Rule& Rule::operator=(const Rule& rhs)
  {
    RulePrivate* n_pointer = new RulePrivate(*this, *rhs.d_pointer);
    delete d_pointer;
    d_pointer = n_pointer;
    return *this;
  }

  uint32_t Rule::getSeqn() const
  {
    return d_pointer->getSeqn();
  }
  
  const String& Rule::getVendorID() const
  {
    return d_pointer->getVendorID();
  }
  
  const String& Rule::getProductID() const
  {
    return d_pointer->getProductID();
  }

  const String& Rule::getSerialNumber() const
  {
    return d_pointer->getSerialNumber();
  }
  
  const String& Rule::getDeviceName() const
  {
    return d_pointer->getDeviceName();
  }
  
  const String& Rule::getDeviceHash() const
  {
    return d_pointer->getDeviceHash();
  }
  
  const StringVector& Rule::getDevicePorts() const
  {
    return d_pointer->getDevicePorts();
  }
  
  const int Rule::getDeviceConfigurations() const
  {
    return d_pointer->getDeviceConfigurations();
  }

  const std::vector<USBInterfaceType>& Rule::getInterfaceTypes() const
  {
    return d_pointer->getInterfaceTypes();
  }
  
  Rule::Target Rule::getTarget() const
  {
    return d_pointer->getTarget();
  }
  
  const String& Rule::getAction() const
  {
    return d_pointer->getAction();
  }
  
  const std::chrono::steady_clock::time_point Rule::getTimePointAdded() const
  {
    return d_pointer->getTimePointAdded();
  }
  
  uint32_t Rule::getTimeoutSeconds() const
  {
    return d_pointer->getTimeoutSeconds();
  }

  bool Rule::appliesTo(Pointer<const Rule> rhs) const
  {
    return appliesTo(*rhs);
  }
  
  bool Rule::appliesTo(const Rule& rhs) const
  {
    return d_pointer->appliesTo(rhs);
  }

  bool Rule::isImplicit() const
  {
    return d_pointer->getSeqn() == Rule::SeqnDefault;
  }
  
  void Rule::setSeqn(uint32_t seqn)
  {
    d_pointer->setSeqn(seqn);
    return;
  }

  void Rule::setVendorID(const String& vendor_id)
  {
    d_pointer->setVendorID(vendor_id);
    return;
  }
  
  void Rule::setProductID(const String& product_id)
  {
    d_pointer->setProductID(product_id);
    return;
  }
  
  void Rule::setSerialNumber(const String& serial_number)
  {
    d_pointer->setSerialNumber(serial_number);
    return;
  }

  void Rule::setDeviceName(const String& device_name)
  {
    d_pointer->setDeviceName(device_name);
    return;
  }
  
  void Rule::setDeviceHash(const String& device_hash)
  {
    d_pointer->setDeviceHash(device_hash);
    return;
  }

  void Rule::setDevicePorts(const StringVector& device_ports)
  {
    d_pointer->setDevicePorts(device_ports);
    return;
  }

  void Rule::setDeviceConfigurations(int num_configurations)
  {
    d_pointer->setDeviceConfigurations(num_configurations);
    return;
  }

  void Rule::setInterfaceTypes(const std::vector<USBInterfaceType>& interface_types)
  {
    d_pointer->setInterfaceTypes(interface_types);
    return;
  }

  StringVector& Rule::refDevicePorts()
  {
    return d_pointer->refDevicePorts();
  }

  std::vector<USBInterfaceType>& Rule::refInterfaceTypes()
  {
    return d_pointer->refInterfaceTypes();
  }

  void Rule::setDevicePortsSetOperator(Rule::SetOperator op)
  {
    d_pointer->setDevicePortsSetOperator(op);
    return;
  }

  void Rule::setInterfaceTypesSetOperator(Rule::SetOperator op)
  {
    d_pointer->setInterfaceTypesSetOperator(op);
    return;
  }
  
  void Rule::setTarget(Rule::Target target)
  {
    d_pointer->setTarget(target);
    return;
  }
  
  void Rule::setAction(const String& action)
  {
    d_pointer->setAction(action);
    return;
  }
  
  void Rule::setTimePointAdded(const std::chrono::steady_clock::time_point tp_added)
  {
    d_pointer->setTimePointAdded(tp_added);
    return;
  }
  
  void Rule::setTimeoutSeconds(uint32_t timeout_seconds)
  {
    d_pointer->setTimeoutSeconds(timeout_seconds);
    return;
  }

  void Rule::setConditionSetOperator(Rule::SetOperator op)
  {
    d_pointer->setConditionSetOperator(op);
    return;
  }

  Rule::operator bool() const
  {
    return !(getTarget() == Target::Unknown ||
	     getTarget() == Target::Invalid);
  }

  String Rule::toString(bool invalid) const
  {
    return d_pointer->toString(invalid);
  }

  Rule Rule::fromString(const String& rule_string)
  {
    return RulePrivate::fromString(rule_string);
  }

  Rule::SetOperator Rule::setOperatorFromString(const String& set_operator_string)
  {
    const std::vector<std::pair<String,Rule::SetOperator> > set_operator_ttable = {
      { "all-of", SetOperator::AllOf },
      { "one-of", SetOperator::OneOf },
      { "none-of", SetOperator::NoneOf },
      { "equals", SetOperator::Equals },
      { "equals-ordered", SetOperator::EqualsOrdered }
    };

    for (auto ttable_entry : set_operator_ttable) {
      if (ttable_entry.first == set_operator_string) {
	return ttable_entry.second;
      }
    }

    throw std::runtime_error("Invalid set operator string");
  }

  const String Rule::setOperatorToString(Rule::SetOperator op)
  {
    const std::vector<std::pair<String,Rule::SetOperator> > set_operator_ttable = {
      { "all-of", SetOperator::AllOf },
      { "one-of", SetOperator::OneOf },
      { "none-of", SetOperator::NoneOf },
      { "equals", SetOperator::Equals },
      { "equals-ordered", SetOperator::EqualsOrdered },
      { "<MATCH>", SetOperator::Match }
    };

    for (auto ttable_entry : set_operator_ttable) {
      if (ttable_entry.second == op) {
	return ttable_entry.first;
      }
    }

    throw std::runtime_error("Invalid set operator string");
  }

  Rule::Target Rule::targetFromString(const String& target_string)
  {
    const std::vector<std::pair<String,Rule::Target> > target_ttable = {
      { "allow", Target::Allow },
      { "block", Target::Block },
      { "reject", Target::Reject },
      { "match", Target::Match },
      { "device", Target::Device }
    };

    for (auto ttable_entry : target_ttable) {
      if (ttable_entry.first == target_string) {
	return ttable_entry.second;
      }
    }

    throw std::runtime_error("Invalid rule target string");
  }

  const String Rule::targetToString(Target target)
  {
    const std::vector<std::pair<String,Rule::Target> > target_ttable = {
      { "allow", Target::Allow },
      { "block", Target::Block },
      { "reject", Target::Reject },
      { "match", Target::Match },
      { "device", Target::Device }
    };

    for (auto ttable_entry : target_ttable) {
      if (ttable_entry.second == target) {
	return ttable_entry.first;
      }
    }

    throw std::runtime_error("Invalid rule target string");
  }

  const String Rule::escapeString(const String& string)
  {
    String result;
    const std::locale c_locale("C");

    for (auto it = string.cbegin(); it != string.cend(); ++it) {
      const char c = *it;

      /*
       * Escape any double-quote and backslash characters.
       */
      if (c == '"') {
	result.append("\\");
	result.append("\"");
	continue;
      }
      if (c == '\\') {
	result.append("\\");
	result.append("\\");
	continue;
      }
      /*
       * If the current character is printable in the "C" locale,
       * append it. Otherwise convert it to \xHH form, where HH is
       * the hexadecimal representation of the character value.
       */
      if (std::isprint(c, c_locale)) {
	result.push_back((char)c);
      } else {
	const String hexbyte = numberToString((uint8_t)c, "\\x", 16, 2, '0');
	result.append(hexbyte);
      }
    }

    return result;
  }

  const String Rule::unescapeString(const String& string)
  {
    String result;
    bool escaped = false;
    const std::locale c_locale("C");

    for (auto it = string.cbegin(); it < string.cend(); ++it) {
      const char c = *it;
      /*
       * Handle an escape sequence if needed, otherwise just
       * append the current character.
       */
      if (escaped) {
	switch (c) {
	case '"':
	  result.append("\"");
	  break;
	case '\\':
	  result.append("\\");
	  break;
	case 'x':
	  {
	    /* hexadecimal representation of a byte \xHH */
	    if (std::distance(string.end(), it) >=2) {
	      throw std::runtime_error("Invalid escape sequence");
	    }

	    const char hb[] = { *(it + 1), *(it + 2) };
	    if (!std::isxdigit(hb[0], c_locale) || !std::isxdigit(hb[1], c_locale)) {
	      throw std::runtime_error("Invalid \\xHH escape sequence: HH is not a hexadecimal number");
	    }

	    const String hexbyte(hb, 2);
	    result.push_back((char)stringToNumber<uint8_t>(hexbyte, 16));

	    ++it;
	    ++it;
	  }
	  break;
	default:
	  throw std::runtime_error("Unknown escape sequence");
	}
	escaped = false;
      }
      else {
	if (c == '\\') {
	  escaped = true;
	}
	else {
	  result.push_back(c);
	}
      }
    }

    if (escaped) {
      throw std::runtime_error("Invalid escape sequence");
    }

    return result;
  }

  RulePrivate* Rule::internal()
  {
    return d_pointer;
  }
} /* namespace usbguard */
