#pragma once
#include <build-config.h>
#include "Typedefs.hpp"
#include "RuleSet.hpp"
#include <istream>
#include <ostream>
#include <mutex>

namespace usbguard {
  class RuleSetPrivate
  {
  public:
    RuleSetPrivate(RuleSet& p_instance);

    void load(const String& path);
    void load(std::istream& stream);
    void save(const String& path);
    void save(std::ostream& stream);
    void setDefaultTarget(Rule::Target target);
    void setDefaultAction(const String& action);
    uint32_t appendRule(const Rule& rule, uint32_t parent_seqn = Rule::SeqnLast);
    Pointer<const Rule> getRule(uint32_t seqn);
    bool removeRule(uint32_t seqn);
    Pointer<const Rule> getFirstMatchingRule(Pointer<const Rule> device_rule, uint32_t from_seqn = 1);
    Pointer<Rule> getTimedOutRule();
    uint32_t assignSeqn(Pointer<Rule> rule);
    uint32_t assignSeqn();

  private:
    std::mutex _io_mutex; /* mutex for load/save */
    std::mutex _op_mutex; /* mutex for operations on the rule set */
    RuleSet& _p_instance;
    Rule::Target _default_target;
    String _default_action;
    Atomic<uint32_t> _seqn_next;
    PointerVector<Rule> _rules;
    PointerPQueue<Rule> _rules_timed;
  };
}
