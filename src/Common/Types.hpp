//
// Copyright (C) 2014 Red Hat, Inc.
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
#ifndef _USBFW_TYPES_H_
#define _USBFW_TYPES_H_

#include <string.h>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <queue>

namespace usbguard
{
  template<typename T>
  void operator<<(std::list<T> list, const T& value)
  {
    list.push_back(value);
  }

  template<typename T>
  void operator<<(std::vector<T> list, const T& value)
  {
    list.push_back(value);
  }

  // char strings
  typedef std::string String;
  typedef std::vector<String> StringVector;
  typedef std::list<String> StringList;
  
  template<typename key_type>
  using StringMap = std::map<key_type, String>;

  template<typename value_type>
  using StringKeyMap = std::map<String, value_type>;

  template<typename key_type>
  using StringVectorMap = std::map<key_type, StringVector>;

  // Smart Pointers
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
} /* namespace usbguard */

#endif /* _USBFW_TYPES_H_ */
