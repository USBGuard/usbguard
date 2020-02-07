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
//          Marek Tamaskovic <mtamasko@redhat.com>
//
#pragma once

#include "Logger.hpp"
#include "Typedefs.hpp"

namespace usbguard
{
  namespace Predicates DLL_PUBLIC
  {
    /**
     * @brief Returns true if the source set is a subset of the
     * target set, false otherwise.
     *
     * @param source Source set.
     * @param target Target set.
     * @return True if the source set is a subset of the
     * target set, false otherwise.
     */
    template<typename T>
    bool isSubsetOf(const T& source, const T& target)
    {
      USBGUARD_LOG(Trace) << "generic isSubsetOf";
      return source == target;
    }

    /**
     * @brief Returns true if the source set is a superset of the
     * target set, false otherwise.
     *
     * @todo Implement.
     *
     * @param source Source set.
     * @param target Target set.
     * @return True if the source set is a superset of the
     * target set, false otherwise.
     */
    template<typename T>
    bool isSupersetOf(const T& source, const T& target)
    {
      USBGUARD_LOG(Error) << "Not implemented";
      (void) source;
      (void) target;
      return true;
    }
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
