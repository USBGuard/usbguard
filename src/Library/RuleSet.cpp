#include "Typedefs.hpp"
#include "RuleSetPrivate.hpp"

namespace usbguard {
  RuleSet::RuleSet()
  {
    d_pointer = new RuleSetPrivate(*this);
  }

  RuleSet::~RuleSet()
  {
    delete d_pointer;
  }

  void RuleSet::load(const String& path)
  {
    d_pointer->load(path);
    return;
  }
  
  void RuleSet::load(std::istream& stream)
  {
    d_pointer->load(stream);
    return;
  }
  
  void RuleSet::save(const String& path)
  {
    d_pointer->save(path);
    return;
  }
  
  void RuleSet::save(std::ostream& stream)
  {
    d_pointer->save(stream);
    return;
  }
  
  void RuleSet::setDefaultTarget(Rule::Target target)
  {
    d_pointer->setDefaultTarget(target);
    return;
  }

  void RuleSet::setDefaultAction(const String& action)
  {
    d_pointer->setDefaultAction(action);
    return;
  }

  uint32_t RuleSet::appendRule(const Rule& rule, uint32_t parent_seqn)
  {
    return d_pointer->appendRule(rule, parent_seqn);
  }

  bool RuleSet::removeRule(uint32_t seqn)
  {
    return d_pointer->removeRule(seqn);
  }

  Pointer<const Rule> RuleSet::getFirstMatchingRule(Pointer<const Rule> device_rule, uint32_t from_seqn)
  {
    return d_pointer->getFirstMatchingRule(device_rule, from_seqn);
  }

  Pointer<Rule> RuleSet::getTimedOutRule()
  {
    return d_pointer->getTimedOutRule();
  }

  uint32_t RuleSet::assignSeqn(Pointer<Rule> rule)
  {
    return d_pointer->assignSeqn(rule);
  }
} /* namespace usbguard */
