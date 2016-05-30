//
// Copyright (C) 2016 Red Hat, Inc.
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
#pragma once

#include "Typedefs.hpp"

namespace usbguard
{
  class Interface;
  class Rule;
  class RuleCondition
  {
  public:
    RuleCondition(const String& identifier, const String& parameter, bool negated = false);
    RuleCondition(const String& identifier, bool negated = false);
    RuleCondition(const RuleCondition& rhs);
    virtual ~RuleCondition();

    virtual void init(Interface * const interface_ptr);
    virtual void fini();
    virtual bool update(const Rule& rule) = 0;
    virtual RuleCondition * clone() const = 0;

    bool evaluate(const Rule& rule);
    const String& identifier() const;
    const String& parameter() const;
    bool hasParameter() const;
    bool isNegated() const;
    const String toString() const;

    static RuleCondition * getImplementation(const String& identifier, const String& parameter, bool negated);

  private:
    const String _identifier;
    const String _parameter;
    const bool _negated;
  };
} /*namespace usbguard */

