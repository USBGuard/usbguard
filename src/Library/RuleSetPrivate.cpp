#include "Typedefs.hpp"
#include "RuleSetPrivate.hpp"
#include <stdexcept>

namespace usbguard {
  
  RuleSetPrivate::RuleSetPrivate(RuleSet& p_instance)
    : _p_instance(p_instance)
  {
    _default_target = Rule::Target::Block;
    _default_action = String();
    _seqn_next = Rule::SeqnRoot + 1;
    return;
  }

  void RuleSetPrivate::load(const String& path)
  {
  }
  
  void RuleSetPrivate::load(std::istream& stream)
  {
  }
  
  void RuleSetPrivate::save(const String& path)
  {
  }
  
  void RuleSetPrivate::save(std::ostream& stream)
  {
  }
  
  void RuleSetPrivate::setDefaultTarget(Rule::Target target)
  {
    _default_target = target;
    return;
  }

  void RuleSetPrivate::setDefaultAction(const String& action)
  {
    _default_action = action;
    return;
  }

  uint32_t RuleSetPrivate::appendRule(const Rule& rule, uint32_t parent_seqn)
  {
    auto rule_ptr = makePointer<Rule>(rule);

    /* Assign a unique sequence number to the rule */
    assignSeqn(rule_ptr);

    /* Set time */
    rule_ptr->setTimePointAdded(std::chrono::steady_clock::now());

    /* Append the rule to the main rule table */
    if (parent_seqn == Rule::SeqnLast) {
      _rules.push_back(rule_ptr);
    }
    else if (parent_seqn == 0) {
      _rules.insert(_rules.begin(), rule_ptr);
    }
    else {
      bool parent_found = false;
      for (auto it = _rules.begin(); it != _rules.end(); ++it) {
	const Rule& rule = **it;
	if (rule.getSeqn() == parent_seqn) {
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
    if (rule_ptr->getTimeoutSeconds() > 0) {
      _rules_timed.push(rule_ptr);
    }

    return rule_ptr->getSeqn();
  }

  bool RuleSetPrivate::removeRule(uint32_t seqn)
  {
    for (auto it = _rules.begin(); it != _rules.end(); ++it) {
      auto const& rule_ptr = *it;
      if (rule_ptr->getSeqn() == seqn) {
	_rules.erase(it);
	return true;
      }
    }
    /* FIXME: Remove the rule from the priority queue too */
    return false;
  }

  Pointer<const Rule> RuleSetPrivate::getFirstMatchingRule(Pointer<const Rule> device_rule, uint32_t from_seqn)
  {
    for (auto const& rule_ptr : _rules) {
      if (rule_ptr->appliesTo(device_rule)) {
	return rule_ptr;
      }
    }

    Pointer<Rule> default_rule = makePointer<Rule>();

    default_rule->setSeqn(Rule::SeqnDefault);
    default_rule->setTarget(_default_target);
    default_rule->setAction(_default_action);

    return default_rule;
  }

  Pointer<Rule> RuleSetPrivate::getTimedOutRule()
  {
    if (_rules_timed.size() < 1) {
      return nullptr;
    }

    Pointer<Rule> oldest_rule = _rules_timed.top();
    std::chrono::steady_clock::time_point tp_current =	\
      std::chrono::steady_clock::now();

    if ((tp_current - oldest_rule->getTimePointAdded()) \
	< std::chrono::seconds(oldest_rule->getTimeoutSeconds())) {
      return nullptr;
    } else {
      _rules_timed.pop();
    }

    return oldest_rule;
  }

  uint32_t RuleSetPrivate::assignSeqn(Pointer<Rule> rule)
  {
    rule->setSeqn(_seqn_next++);
    return rule->getSeqn();
  }
} /* namespace usbguard */
