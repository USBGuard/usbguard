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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "Actions.hpp"
#include <pegtl.hh>

using namespace pegtl;

namespace usbguard
{
  namespace RuleParser
  {
    /*
     * Rule language keywords
     */
    struct str_allow : pegtl_string_t("allow") {};
    struct str_block : pegtl_string_t("block") {};
    struct str_reject : pegtl_string_t("reject") {};
    struct str_match : pegtl_string_t("match") {};
    struct str_device : pegtl_string_t("device") {};

    struct str_name : pegtl_string_t("name") {};
    struct str_hash : pegtl_string_t("hash") {};
    struct str_parent_hash : pegtl_string_t("parent-hash") {};
    struct str_via_port : pegtl_string_t("via-port") {};
    struct str_with_interface : pegtl_string_t("with-interface") {};
    struct str_serial : pegtl_string_t("serial") {};
    struct str_if : pegtl_string_t("if") {};
    struct str_id : pegtl_string_t("id") {};

    struct str_all_of : pegtl_string_t("all-of") {};
    struct str_one_of : pegtl_string_t("one-of") {};
    struct str_none_of : pegtl_string_t("none-of") {};
    struct str_equals : pegtl_string_t("equals") {};
    struct str_equals_ordered : pegtl_string_t("equals-ordered") {};

    /*
     * Generic rule attribute
     */
    struct multiset_operator
      : sor<str_all_of, str_one_of, str_none_of, str_equals_ordered, str_equals> {};

    template<class attribute_value_rule>
    struct attribute_value_multiset
      : seq<opt<multiset_operator, plus<ascii::blank>>,
        one<'{'>, star<ascii::blank>,
        list<attribute_value_rule, plus<ascii::blank>>,
        star<ascii::blank>, one<'}'>> {};

    template<class attribute_identifier, class attribute_value_rule>
    struct rule_attribute
      : seq<attribute_identifier, plus<ascii::blank>,
        sor<attribute_value_multiset<attribute_value_rule>,
        attribute_value_rule>> {};

    /*
     * Bounded string value
     */
    struct escaped_single
      : one<'a', 'b', 'f', 'n', 'r', 't', 'v', '\\', '\'', '0', '\n'> {};

    struct escaped_decbyte
      : if_must<ascii::digit, rep_opt<2, ascii::digit>> {};

    struct escaped_hexbyte
      : if_must<one<'x'>, rep<2, ascii::xdigit>> {};

    template<char boundary_escape>
    struct character_escaped
      : if_must<one<'\\'>, sor<escaped_hexbyte, escaped_decbyte, escaped_single, one<boundary_escape>>> {};

    struct character_regular
      : not_one<'\r', '\n'> {};

    template<char boundary_escape>
    struct character
      : sor<character_escaped<boundary_escape>, character_regular> {};

    template<char boundary_start, char boundary_end = boundary_start>
    struct bounded_string
      : if_must<one<boundary_start>, until<one<boundary_end>, character<boundary_end>>> {};

    struct string_value
      : bounded_string<'"'> {};

    /*
     * Rule conditions
     */
    struct condition_argument
      : bounded_string<'(', ')'> {};

    struct condition_identifier
      : seq<ranges<'a', 'z', 'A', 'Z', '-'>, star<ranges<'a', 'z', 'A', 'Z', '0', '9', '-'>>> {};

    struct negation
      : one<'!'> {};

    struct condition
      : seq<opt<negation>, condition_identifier, opt<condition_argument>> {};

    /*
     * Device ID
     */
    struct hex4
      : rep<4, ascii::xdigit> {};

    struct device_vid : sor<hex4, one<'*'>> {};
    struct device_pid : sor<hex4, one<'*'>> {};

    struct device_id_value
      : seq<device_vid, one<':'>, device_pid> {};

    struct device_id
      : device_id_value {};

    /*
     * USB interface notation
     */
    struct hex2
      : rep<2, ascii::xdigit> {};

    struct hex2orAsterisk
      : sor<hex2, one<'*'>> {};

    struct interface_value
      : seq<hex2, one<':'>, hex2orAsterisk, one<':'>, hex2orAsterisk> {};

    /*
     * Device attributes
     */
    struct id_attribute
      : action<id_actions, rule_attribute<str_id, device_id_value>> {};

    struct name_attribute
      : action<name_actions, rule_attribute<str_name, string_value>> {};

    struct hash_attribute
      : action<hash_actions, rule_attribute<str_hash, string_value>> {};

    struct parent_hash_attribute
      : action<parent_hash_actions, rule_attribute<str_parent_hash, string_value>> {};

    struct serial_attribute
      : action<serial_actions, rule_attribute<str_serial, string_value>> {};

    struct via_port_attribute
      : action<via_port_actions, rule_attribute<str_via_port, string_value>> {};

    struct with_interface_attribute
      : action<with_interface_actions, rule_attribute<str_with_interface, interface_value>> {};

    struct condition_attribute
      : action<condition_actions, rule_attribute<str_if, condition>> {};

    struct rule_attributes
      : sor<id_attribute,
        name_attribute,
        hash_attribute,
        parent_hash_attribute,
        serial_attribute,
        via_port_attribute,
        with_interface_attribute,
        condition_attribute> {};

    /*
     * Rule target
     */
    struct target
      : sor<str_allow, str_block, str_reject, str_match, str_device> {};

    /*
     * Rule
     */
    struct rule
      : seq<target,
        opt<plus<ascii::blank>, device_id>,
        opt<plus<ascii::blank>, list<rule_attributes, plus<ascii::blank>>>> {};

    /*
     * Grammar entry point
     */
    struct rule_grammar
      : until<eof, must<rule>> {};
  } /* namespace RuleParser */
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
