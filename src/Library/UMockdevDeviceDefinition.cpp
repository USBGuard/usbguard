//
// Copyright (C) 2018 Red Hat, Inc.
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

#include "UMockdevDeviceDefinition.hpp"

#include <usbguard/Logger.hpp>
#include <Common/Utility.hpp>

#include <tao/pegtl.hpp>
#if TAO_PEGTL_VERSION_MAJOR >= 3
  #include <tao/pegtl/contrib/trace.hpp>
#else
  #include <tao/pegtl/contrib/tracer.hpp>
#endif

namespace usbguard
{
  namespace UMockdevParser
  {
    using namespace tao::pegtl;

    /*
     * Each paragraph defines one device.
     * A line starts with a type tag (like 'E'), followed by a colon, followed by either a value or a "key=value" assignment,
     * depending on the type tag. A device description must start with a 'P:' line. Available type tags are:
     *
     *  P:path: device path in sysfs, starting with /devices/; must occur exactly once at the start of device definition
     *  E:key=value: udev property
     *  A:key=value: ASCII sysfs attribute, with backslash-style escaping of \ (\\) and newlines (\n)
     *  H:key=value: binary sysfs attribute, with the value being written as continuous hex string (e. g. 0081FE0A..)
     *  N:devname[=contents]: device node name (without the /dev/ prefix); if contents is given (encoded in a continuous hex string),
     *                        it creates a /dev/devname in the mock environment with the given contents, otherwise the created dev file
     *                        will be a pty; see umockdev_testbed_get_dev_fd for details.
     *  S:linkname: device node symlink (without the /dev/ prefix); ignored right now.
     */

    struct str_path_prefix : TAO_PEGTL_STRING("P:") {};
    struct str_property_prefix : TAO_PEGTL_STRING("E:") {};
    struct str_ascii_attr_prefix : TAO_PEGTL_STRING("A:") {};
    struct str_binary_attr_prefix : TAO_PEGTL_STRING("H:") {};
    struct str_link_prefix : TAO_PEGTL_STRING("L:") {};
    struct str_name_prefix : TAO_PEGTL_STRING("N:") {};

    struct line_rest
      : star<not_at<ascii::eol>, not_at<eof>, ascii::any> {};

    struct devfs_node_value
      : line_rest {};

    struct devfs_node_line
      : seq<str_name_prefix, devfs_node_value> {};

    struct ascii_attr_value
      : line_rest {};

    struct ascii_attr_line
      : seq<str_ascii_attr_prefix, ascii_attr_value> {};

    struct property_value
      : line_rest {};

    struct property_line
      : seq<str_property_prefix, property_value> {};

    struct unprocessed_line
      : seq<sor<str_binary_attr_prefix, str_link_prefix>, line_rest> {};

    struct definition_rest
      : list<sor<devfs_node_line, property_line, ascii_attr_line, unprocessed_line>, ascii::eol> {};

    struct sysfs_path_value
      : line_rest {};

    struct sysfs_path_line_begin
      : str_path_prefix {};

    struct sysfs_path_line
      : seq<sysfs_path_line_begin, sysfs_path_value> {};

    struct empty_line
      : until<ascii::eol, ascii::blank> {};

    struct definition
      : seq<sysfs_path_line, ascii::eol, definition_rest> {};

    struct grammar
      : until<eof, must<sor<definition, empty_line>>> {};

    template<typename Rule>
    struct actions
      : tao::pegtl::nothing<Rule> {};

    using Definitions = std::vector<std::unique_ptr<UMockdevDeviceDefinition>>;

    template<>
    struct actions<sysfs_path_line_begin> {
      template<typename Input>
      static void apply(const Input& in, Definitions& definitions, const std::string& umockdev_name)
      {
        std::unique_ptr<UMockdevDeviceDefinition> definition(new UMockdevDeviceDefinition());
        (void)in;
        definition->setUMockdevName(umockdev_name);
        definitions.push_back(std::move(definition));
      }
    };

    template<>
    struct actions<sysfs_path_value> {
      template<typename Input>
      static void apply(const Input& in, Definitions& definitions, const std::string& umockdev_name)
      {
        (void)umockdev_name;
        std::unique_ptr<UMockdevDeviceDefinition>& definition = definitions.back();
        definition->setSysfsPath(trim(in.string()));
      }
    };

    template<>
    struct actions<devfs_node_value> {
      template<typename Input>
      static void apply(const Input& in, Definitions& definitions, const std::string& umockdev_name)
      {
        (void)umockdev_name;
        std::unique_ptr<UMockdevDeviceDefinition>& definition = definitions.back();
        definition->setDevfsNode(trim(in.string()));
      }
    };

#if 0
    template<>
    struct actions<ascii_attr_value> {
      template<typename Input>
      static void apply(const Input& in, Definitions& definitions, const std::string& umockdev_name)
      {
        (void)umockdev_name;
        const std::string in_trimmed = trim(in.string());
        const auto separator_pos = in_trimmed.find_first_of("=");
        const std::string key = in_trimmed.substr(0, separator_pos);
        const std::string value = in_trimmed.substr(separator_pos + 1);
        std::unique_ptr<UMockdevDeviceDefinition>& definition = definitions.back();
      }
    };
#endif

    template<>
    struct actions<property_value> {
      template<typename Input>
      static void apply(const Input& in, Definitions& definitions, const std::string& umockdev_name)
      {
        (void)umockdev_name;
        const std::string in_trimmed = trim(in.string());
        const auto separator_pos = in_trimmed.find_first_of("=");
        const std::string key = in_trimmed.substr(0, separator_pos);
        const std::string value = in_trimmed.substr(separator_pos + 1);
        USBGUARD_LOG(Debug) << "Property: key=" << key << " value=" << value;

        if (key != "DEVTYPE") {
          return;
        }

        std::unique_ptr<UMockdevDeviceDefinition>& definition = definitions.back();
        definition->setDeviceType(value);
      }
    };

    template<>
    struct actions<definition> {
      template<typename Input>
      static void apply(const Input& in, Definitions& definitions, const std::string& umockdev_name)
      {
        (void)umockdev_name;
        std::unique_ptr<UMockdevDeviceDefinition>& definition = definitions.back();
        definition->setUMockdevDefinition(trim(in.string()));
      }
    };
  }

  UMockdevDeviceDefinition::UMockdevDeviceDefinition()
  {
  }

  const std::string& UMockdevDeviceDefinition::getUMockdevName() const
  {
    return _name;
  }

  void UMockdevDeviceDefinition::setUMockdevName(const std::string& name)
  {
    _name = name;
  }

  const std::string& UMockdevDeviceDefinition::getSysfsPath() const
  {
    return _sysfs_path;
  }

  void UMockdevDeviceDefinition::setSysfsPath(const std::string& sysfs_path)
  {
    _sysfs_path = sysfs_path;
  }

  const std::string& UMockdevDeviceDefinition::getDevfsNode() const
  {
    return _devfs_node;
  }

  void UMockdevDeviceDefinition::setDevfsNode(const std::string& devfs_node)
  {
    _devfs_node = devfs_node;
  }

  bool UMockdevDeviceDefinition::hasDevfsNode() const
  {
    return !_devfs_node.empty();
  }

  const std::string& UMockdevDeviceDefinition::getDeviceType() const
  {
    return _device_type;
  }

  void UMockdevDeviceDefinition::setDeviceType(const std::string& device_type)
  {
    _device_type = device_type;
  }


  const std::string& UMockdevDeviceDefinition::getUMockdevDefinition() const
  {
    return _definition;
  }

  void UMockdevDeviceDefinition::setUMockdevDefinition(const std::string& definition)
  {
    _definition = definition;
  }

  UMockdevDeviceDefinition::operator bool () const
  {
    return !_definition.empty();
  }

  std::vector<std::unique_ptr<UMockdevDeviceDefinition>> UMockdevDeviceDefinition::parseFromFile(const std::string& filepath,
      bool sort_by_hierarchy)
  {
    const std::string umockdev_name = filenameFromPath(filepath, /*include_extension=*/true);
    std::vector<std::unique_ptr<UMockdevDeviceDefinition>> definitions;

    try {
      USBGUARD_LOG(Debug) << "Parsing umockdev definitions from " << filepath;
      tao::pegtl::file_input<> input(filepath);
      tao::pegtl::parse<UMockdevParser::grammar, UMockdevParser::actions>(input, definitions, umockdev_name);
      USBGUARD_LOG(Debug) << "Parsed " << definitions.size() << " definition(s)";
    }
    catch (...) {
      USBGUARD_LOG(Error) << "UMockdevDeviceDefinition: " << filepath << ": parsing failed at line <LINE>";
      throw;
    }

    const auto lambdaSysfsPathHierarchyCompare = [](const std::unique_ptr<UMockdevDeviceDefinition>& a,
    const std::unique_ptr<UMockdevDeviceDefinition>& b) {
      const std::string full_a = a->getSysfsPath();
      const std::string full_b = b->getSysfsPath();
      const std::size_t component_count_a = countPathComponents(full_a);
      const std::size_t component_count_b = countPathComponents(full_b);
      USBGUARD_LOG(Debug) << "c_c_a=" << component_count_a << " c_c_b=" << component_count_b;

      if (component_count_a < component_count_b) {
        return true;
      }
      else if (component_count_a > component_count_b) {
        return false;
      }

      const std::string base_a = filenameFromPath(full_a, /*include_extension=*/true);
      const std::string base_b = filenameFromPath(full_b, /*include_extension=*/true);
      const bool a_has_usb_prefix = hasPrefix("usb", base_a);
      const bool b_has_usb_prefix = hasPrefix("usb", base_b);
      USBGUARD_LOG(Debug) << "a_p=" << a_has_usb_prefix << " b_p=" << b_has_usb_prefix;

      if (a_has_usb_prefix) {
        if (!b_has_usb_prefix) {
          return true;
        }
      }
      else {
        if (b_has_usb_prefix) {
          return false;
        }
      }

      return base_a < base_b;
    };

    if (sort_by_hierarchy) {
      USBGUARD_LOG(Debug) << "Sorting definitions";
      std::sort(definitions.begin(), definitions.end(), lambdaSysfsPathHierarchyCompare);
    }

    return definitions;
  }

  /*
   * FIXME (easy): Deduplicate the code here a bit. The parseFrom* methods could be reimplemented to
   *               share the parsing & sorting code. The difference is only the source. Let's create
   *               a low-level parsing method template that will generate one or the other implementation.
   *               Another option is to deduplicate this by creating a shared implementation that parses
   *               the data only from memory and handle the source name from the specialized methods.
   */
  std::vector<std::unique_ptr<UMockdevDeviceDefinition>> UMockdevDeviceDefinition::parseFromString(
      const std::string& definitions_string, bool sort_by_hierarchy)
  {
    const std::string umockdev_name = "<string>";
    std::vector<std::unique_ptr<UMockdevDeviceDefinition>> definitions;

    try {
      tao::pegtl::string_input<> input(definitions_string, "<string>");
#if TAO_PEGTL_VERSION_MAJOR >= 3
      tao::pegtl::complete_trace<UMockdevParser::grammar, UMockdevParser::actions>(input, definitions, umockdev_name);
#else
      tao::pegtl::parse<UMockdevParser::grammar, UMockdevParser::actions, tao::pegtl::tracer>(input, definitions, umockdev_name);
#endif
    }
    catch (...) {
      USBGUARD_LOG(Error) << "UMockdevDeviceDefinition: " << "<string>" << ": parsing failed at line <LINE>";
      throw;
    }

    const auto lambdaSysfsPathHierarchyCompare = [](const std::unique_ptr<UMockdevDeviceDefinition>& a,
    const std::unique_ptr<UMockdevDeviceDefinition>& b) {
      return (a->getSysfsPath().size() < b->getSysfsPath().size() ||
          b->getSysfsPath() <= b->getSysfsPath());
    };

    if (sort_by_hierarchy) {
      std::sort(definitions.begin(), definitions.end(), lambdaSysfsPathHierarchyCompare);
    }

    return definitions;
  }
}
/* vim: set ts=2 sw=2 et */
