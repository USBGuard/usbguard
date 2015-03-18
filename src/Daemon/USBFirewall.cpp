#include "USBFirewall.hpp"
#include "Daemon/Exceptions.hpp"

namespace usbguard
{
  Firewall::Firewall()
  {
    _default_target = Target::Deny;
    _default_action = String();
    _rule_seqn_next = 1; /* 0 is allocated for inserting at the begining */
  }

  void Firewall::setDefaultTarget(Target target)
  {
    _default_target = target;
    return;
  }

  void Firewall::setDefaultAction(const String& action)
  {
    _default_action = action;
    return;
  }

  unsigned int Firewall::appendRule(const Rule& rule, unsigned int parent_seqn)
  {
    auto rule_ptr = makePointer<Rule>(rule);

    /* Assign a unique sequence number to the rule */
    assignSeqn(*rule_ptr);

    /* Set time */
    getCurrentTime(&rule_ptr->ts_added);

    /* Append the rule to the main rule table */
    if (parent_seqn == Rule::LastSeqn) {
      _rules.push_back(rule_ptr);
    }
    else if (parent_seqn == 0) {
      _rules.insert(_rules.begin(), rule_ptr);
    }
    else {
      bool parent_found = false;
      for (auto it = _rules.begin(); it != _rules.end(); ++it) {
	const Rule& rule = **it;
	if (rule.rule_seqn == parent_seqn) {
	  _rules.insert(it+1, rule_ptr);
	  parent_found = true;
	  break;
	}
      }
      if (!parent_found) {
	throw std::runtime_error("Invalid parent_seqn");
      }
    }

    /* If the rule is timed, put it into the priority queue */
    if (rule_ptr->timeout_sec > 0) {
      _rules_timed.push(rule_ptr);
    }

    return rule_ptr->rule_seqn;
  }

  unsigned int Firewall::assignSeqn(Rule& rule)
  {
    rule.rule_seqn = _rule_seqn_next++;
    return rule.rule_seqn;
  }

  bool Firewall::removeRule(unsigned int rule_seqn)
  {
    for (auto it = _rules.begin(); it != _rules.end(); ++it) {
      auto const& rule_ptr = *it;
      if (rule_ptr->rule_seqn == rule_seqn) {
	_rules.erase(it);
	return true;
      }
    }

    /* FIXME: Remove the rule from the priority queue too */

    return false;
  }

  Pointer<const Firewall::Rule> Firewall::getFirstMatchingRule(const Rule& device_rule)
  {
    for (auto const& rule_ptr : _rules) {
      if (rule_ptr->appliesTo(device_rule)) {
	return rule_ptr;
      }
    }

    Pointer<Rule> default_rule = makePointer<Rule>();

    default_rule->rule_seqn = Rule::DefaultSeqn;
    default_rule->target = _default_target;
    default_rule->action = _default_action;

    return default_rule;
  }

  Pointer<Firewall::Rule> Firewall::getTimedOutRule()
  {
    if (_rules_timed.size() < 1) {
      return nullptr;
    }

    Pointer<Rule> oldest_rule = _rules_timed.top();

    struct timespec ts_current;
    getCurrentTime(&ts_current);

    if (tsSecondDifference(&oldest_rule->ts_added, &ts_current) < oldest_rule->timeout_sec) {
      return nullptr;
    } else {
      _rules_timed.pop();
    }

    return oldest_rule;
  }

} /* namespace usbguard */
