#pragma once
#include <Rule.hpp>
#include <istream>
#include <ostream>

namespace usbguard {
  class RuleSetPrivate;
  class RuleSet
  {
  public:
    /**
     * Construct an empty ruleset.
     */
    RuleSet();
    ~RuleSet();

    /**
     * Load a ruleset from a file at `path'.
     * Internally, this opens an input file stream and calls load(std::istream& stream).
     */
    void load(const String& path);

    /**
     * Load a ruleset from an input stream.
     * The stream is read using std::getline() and each line is parsed as a separate rule.
     * Empty lines are skipped.
     */
    void load(std::istream& stream);

    /**
     * Save the ruleset to a file at `path'.
     * If a file exists at `path', it will be overwritten. Internally, this opens an output
     * stream and calls save(std::ostream& stream).
     */
    void save(const String& path);

    /**
     * Write the ruleset to an output stream.
     * Each rule is serialized to it's string representation and written line by line to
     * the output stream.
     */
    void save(std::ostream& stream);

    /**
     * Set an implicit default target which will be used if there's no match for a device
     * rule.
     */
    void setDefaultTarget(Rule::Target target);

    /**
     * Set an implicit default action which will be used if there's no match for a device
     * rule.
     */
    void setDefaultAction(const String& action);

    /**
     * Assign a sequence number to a rule and append it to the rule set.
     * If `parent_seqn' is not specified, the rule will be appended at the end od the ruleset.
     * The method returns the sequence number assigned to the rule.
     */
    uint32_t appendRule(const Rule& rule, uint32_t parent_seqn = Rule::SeqnLast);

    /**
     * Get a rule pointer to a rule with the specified sequence number.
     * Returns nullptr if no such rule exists.
     */
    Pointer<const Rule> getRule(uint32_t seqn);
    
    /**
     * Remove a rule from the ruleset.
     * The method returns true if a rule was removed and false otherwise.
     */
    bool removeRule(uint32_t seqn);

    /**
     * Find first rule in the ruleset which matched the specified device rule.
     * If `from_seqn' isn't specified, the method searches from the beginning of the ruleset.
     * If no matching rule is found, nullptr is returned.
     */
    Pointer<const Rule> getFirstMatchingRule(Pointer<const Rule> device_rule, uint32_t from_seqn = 1);

    /**
     * Get the oldest rule that timed out and should be removed from the ruleset.
     * Returns nullptr if there are not timed out rules.
     */
    Pointer<Rule> getTimedOutRule();

    /**
     * Assign a unique sequence number to a rule.
     * Return the assigned sequence number.
     */
    uint32_t assignSeqn(Pointer<Rule> rule);

    /**
     * Generate a unique sequence number.
     */
    uint32_t assignSeqn();

  private:
    RuleSetPrivate* d_pointer;
  };
}
