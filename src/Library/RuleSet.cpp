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
#include "Typedefs.hpp"
#include "RuleSetPrivate.hpp"

namespace usbguard {
  RuleSet::RuleSet(Interface * const interface_ptr)
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
    RuleSetPrivate * n_pointer = new RuleSetPrivate(*this, *rhs.d_pointer);
    delete d_pointer;
    d_pointer = n_pointer;
    return *this;
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
  
  void RuleSet::save(const String& path) const
  {
    d_pointer->save(path);
    return;
  }
  
  void RuleSet::save(std::ostream& stream) const
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

  Pointer<const Rule> RuleSet::getRule(uint32_t seqn)
  {
    return d_pointer->getRule(seqn);
  }

  bool RuleSet::removeRule(uint32_t seqn)
  {
    return d_pointer->removeRule(seqn);
  }

  Pointer<const Rule> RuleSet::getFirstMatchingRule(Pointer<const Rule> device_rule, uint32_t from_seqn)
  {
    return d_pointer->getFirstMatchingRule(device_rule, from_seqn);
  }

  PointerVector<const Rule> RuleSet::getRules()
  {
    return d_pointer->getRules();
  }

  Pointer<Rule> RuleSet::getTimedOutRule()
  {
    return d_pointer->getTimedOutRule();
  }

  uint32_t RuleSet::assignSeqn(Pointer<Rule> rule)
  {
    return d_pointer->assignSeqn(rule);
  }

  uint32_t RuleSet::assignSeqn()
  {
    return d_pointer->assignSeqn();
  }

} /* namespace usbguard */
