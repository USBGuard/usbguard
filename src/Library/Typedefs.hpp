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
#pragma once

#include <string.h>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <atomic>

namespace usbguard {
  template<typename T>
  bool matches(const T& a, const T& b)
  {
    return a == b;
  }

  template<>
  bool matches(const std::string& a, const std::string& b);

  /*
   * Smart Pointer typedefs and custom operations
   */
  template<typename pointer_type>
  using Pointer = std::shared_ptr<pointer_type>;

  template<typename pointer_type, typename deleter_type = std::default_delete<pointer_type> >
  using UniquePointer = std::unique_ptr<pointer_type, deleter_type>;

  template<typename pointer_type>
  using PointerVector = std::vector<Pointer<pointer_type> >;

  template<typename key_type, typename pointer_type>
  using PointerMap = std::map<key_type, Pointer<pointer_type> >;

  template<typename pointer_type>
  using PointerPQueue = std::priority_queue<Pointer<pointer_type> >;

  template<typename pointer_type, typename... Args>
  static inline Pointer<pointer_type> makePointer(Args&&... args)
  {
    return std::make_shared<pointer_type>(std::forward<Args>(args)...);
  }

  template<typename pointer_type, typename... Args>
  static inline UniquePointer<pointer_type> makeUniquePointer(Args&&... args)
  {
    return std::move(std::unique_ptr<pointer_type>(new pointer_type(std::forward<Args>(args)...)));
  }

  namespace MapOp {
    template<typename map_type>
    typename map_type::mapped_type findOne(const map_type& map, const typename map_type::key_type& key)
    {
      auto it = map.find(key);
      if (it == map.end()) {
	return typename map_type::mapped_type();
      } else {
	return it->second;
      }
    }
  }

  namespace PointerMapOp {
    template<typename key_type, typename pointer_type>
    Pointer<pointer_type> findOne(const PointerMap<key_type, pointer_type>& map,
				  const key_type& key)
    {
      auto it = map.find(key);
      if (it == map.end()) {
	return Pointer<pointer_type>(nullptr);
      } else {
	return it->second;
      }
    }
  }

  /*
   * Atomic
   */
  template<typename integral_type>
  using Atomic = std::atomic<integral_type>;

  /*
   * Symbol visibility
   */
  #if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define DLL_LOCAL
  #else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
  #endif
} /* namespace usbguard */
