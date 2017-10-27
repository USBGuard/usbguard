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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "Typedefs.hpp"
#include "RuleSetPrivate.hpp"

namespace usbguard
{
  RuleSet::RuleSet(Interface* const interface_ptr)
  {
    d_pointer = new RuleSetPrivate(*this, interface_ptr);
  }

  RuleSet::~RuleSet()
  {
    delete d_pointer;
    d_pointer = nullptr;
  }

  RuleSet::RuleSet(const RuleSet& rhs)
  {
    d_pointer = new RuleSetPrivate(*this, *rhs.d_pointer);
  }

  const RuleSet& RuleSet::operator=(const RuleSet& rhs)
  {
    RuleSetPrivate* n_pointer = new RuleSetPrivate(*this, *rhs.d_pointer);
    delete d_pointer;
    d_pointer = n_pointer;
    return *this;
  }

  void RuleSet::load(const std::string& path)
  {
    d_pointer->load(path);
  }

  void RuleSet::load(std::istream& stream)
  {
    d_pointer->load(stream);
  }

  void RuleSet::save(const std::string& path) const
  {
    d_pointer->save(path);
  }

  void RuleSet::save(std::ostream& stream) const
  {
    d_pointer->save(stream);
  }

  void RuleSet::setDefaultTarget(Rule::Target target)
  {
    d_pointer->setDefaultTarget(target);
  }

  Rule::Target RuleSet::getDefaultTarget() const
  {
    return d_pointer->getDefaultTarget();
  }

  void RuleSet::setDefaultAction(const std::string& action)
  {
    d_pointer->setDefaultAction(action);
  }

  uint32_t RuleSet::appendRule(const Rule& rule, uint32_t parent_id)
  {
    return d_pointer->appendRule(rule, parent_id);
  }

  uint32_t RuleSet::upsertRule(const Rule& match_rule, const Rule& new_rule, const bool parent_insensitive)
  {
    return d_pointer->upsertRule(match_rule, new_rule, parent_insensitive);
  }

  std::shared_ptr<Rule> RuleSet::getRule(uint32_t id)
  {
    return d_pointer->getRule(id);
  }

  bool RuleSet::removeRule(uint32_t id)
  {
    return d_pointer->removeRule(id);
  }

  std::shared_ptr<Rule> RuleSet::getFirstMatchingRule(std::shared_ptr<const Rule> device_rule, uint32_t from_id) const
  {
    return d_pointer->getFirstMatchingRule(device_rule, from_id);
  }

  std::vector<std::shared_ptr<const Rule>> RuleSet::getRules()
  {
    return d_pointer->getRules();
  }

  uint32_t RuleSet::assignID(std::shared_ptr<Rule> rule)
  {
    return d_pointer->assignID(rule);
  }

  uint32_t RuleSet::assignID()
  {
    return d_pointer->assignID();
  }

} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
