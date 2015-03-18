#ifndef USBFW_FIREWALL_HPP
#define USBFW_FIREWALL_HPP

#include "Common/Types.hpp"
#include "Daemon/Exceptions.hpp"
#include "Common/Logging.hpp"

#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <limits>
#include <atomic>

namespace usbguard
{
  class Firewall
  {
  public:
    enum Target {
      Allow,
      Deny,
      Reject,
      Unknown,
      Invalid
    };

    struct Rule
    {
      static const unsigned int LastSeqn = std::numeric_limits<unsigned int>::max() - 1;
      static const unsigned int DefaultSeqn = std::numeric_limits<unsigned int>::max();

      unsigned int rule_seqn; // rule sequence number
      String id_vendor; // vendor id from /sys
      String id_product; // product id from /sys
      String name; // product name from /sys
      
      String hash; // hash of some device attributes (empty means don't apply to filtering)
      StringVector id_ports; // sysnames of the device (empty means don't apply to filtering)
      String usb_class;
      String ref_syspath; // empty, or pointing to an existing system device
      Target target; // allow, deny, invalid
      String action; // run program, limit, ...

      struct timespec ts_added; // time when the rule was added
      unsigned int timeout_sec; // rule timeout in seconds

      Rule() {
	target = Target::Invalid;
	ts_added = { 0, 0 };
	timeout_sec = 0;
      }

      operator bool() const
      {
	return (target != Target::Invalid &&
		target != Target::Unknown);
      }

      bool appliesTo(const Rule& rhs)
      {
	log->debug("appliesTo? rule {} vs rule {}", rule_seqn, rhs.rule_seqn);
	/*
	 * If a this set of rules contains the rhs rule, return true. Otherwise false.
	 * Ignored fields: rule_seqn, target, action, ts_added, timeout_sec, ref_syspath
	 */
	if (!id_vendor.empty()) {
	  if (id_vendor != rhs.id_vendor) {
	    log->debug("appliesTo: vendor mismatch: '{}' != '{}'", id_vendor, rhs.id_vendor);
	    return false;
	  }
	}
	if (!id_product.empty()) {
	  if (id_product != rhs.id_product) {
	    log->debug("appliesTo: product mismatch: '{}' != '{}'", id_product, rhs.id_product);
	    return false;
	  }
	}
	if (!name.empty()) {
	  if (name != rhs.name) {
	    log->debug("appliesTo: name mismatch: '{}' != '{}'", name, rhs.name);
	    return false;
	  }
	}
	if (!hash.empty()) {
	  if (hash != rhs.hash) {
	    log->debug("appliesTo: hash mismatch: '{}' != '{}'", hash, rhs.hash);
	    return false;
	  }
	}
	if (id_ports.size() > 0) {
	  bool match = false;
	  for (auto const& port : id_ports) {
	    for (auto const& rhs_port : rhs.id_ports) {
	      if (port == rhs_port) {
		match = true;
		break;
	      }
	    }
	    if (match) {
	      break;
	    }
	  }
	  if (!match) {
	    log->debug("appliesTo: port mismatch");
	    return false;
	  }
	}
	log->debug("appliesTo: ALL MATCHED!");
	return true;
      }

      bool operator<(const Rule& rhs) const
      {
	return timeout_sec > rhs.timeout_sec;
      }
    };

    Firewall();

    void setDefaultTarget(Target target);
    void setDefaultAction(const String& action);
    unsigned int appendRule(const Rule& rule, unsigned int parent_seqn = Rule::LastSeqn);
    /**
     * Remove a rule given it's sequence number. Returns true if a rule was
     * erased, false otherwise.
     */
    bool removeRule(unsigned int rule_seqn);

    /**
     * Find the first matching rule given a device derived rule.
     */
    Pointer<const Rule> getFirstMatchingRule(const Rule& device_rule);

    /**
     * Return the oldest rule that already timed out. Returns nullptr if there
     * are no timed out rules.
     */
    Pointer<Rule> getTimedOutRule();

    /**
     * Assign a sequence number to a rule
     */
    unsigned int assignSeqn(Rule& rule);

  protected:
    static void getCurrentTime(struct timespec *ts)
    {
      if (clock_gettime(CLOCK_MONOTONIC, ts) != 0) {
	throw std::runtime_error("clock_gettime failed");
      }
      return;
    }

    static unsigned int tsSecondDifference(const struct timespec *a, const struct timespec *b)
    {
      return abs(a->tv_sec - b->tv_sec);
    }

  private:
    Target _default_target;
    String _default_action;
    std::atomic<unsigned int> _rule_seqn_next;

    PointerVector<Rule> _rules;
    PointerPQueue<Rule> _rules_timed;
  };
} /* namespace usbguard */

#endif /* USBFW_FIREWALL_HPP */
