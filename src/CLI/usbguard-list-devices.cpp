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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard.hpp"
#include "usbguard-list-devices.hpp"

#include "usbguard/IPCClient.hpp"

#include <iostream>
#include <map>
#include <vector>

namespace usbguard
{
  static const char* options_short = "habt";

  static const struct ::option options_long[] = {
    { "help", no_argument, nullptr, 'h' },
    { "allowed", no_argument, nullptr, 'a' },
    { "blocked", no_argument, nullptr, 'b' },
    { "tree", no_argument, nullptr, 't' },
    { nullptr, 0, nullptr, 0 }
  };

  static void showHelp(std::ostream& stream)
  {
    stream << " Usage: " << usbguard_arg0 << " list-devices [OPTIONS]" << std::endl;
    stream << std::endl;
    stream << " Options:" << std::endl;
    stream << "  -a, --allowed  List allowed devices." << std::endl;
    stream << "  -b, --blocked  List blocked devices." << std::endl;
    stream << "  -t, --tree     List devices in a tree format." << std::endl;
    stream << "  -h, --help     Show this help." << std::endl;
    stream << std::endl;
  }

  /**
   * @brief Prints list of devices in a classic format
   *
   * @param rules Device rules to print
   */
  static void classicFormat(const std::vector<Rule>& rules)
  {
    for (const auto& rule : rules) {
      std::cout << rule.getRuleID() << ": " << rule.toString() << std::endl;
    }
  }

  /**
   * @brief Recursively prints the tree nodes
   *
   * @param tree Rules organized into a tree structure
   * @param node Node of a tree
   * @param prefix Helper string used for prefixing the output
   */
  static void printNode(
    const std::map<std::string, std::pair<Rule, std::vector<std::string>>>& tree,
    const std::pair<Rule, std::vector<std::string>>& node,
    const std::string& prefix)
  {
    const auto& rule = node.first;
    const auto& children = node.second;

    if (rule) {
      std::cout << rule.getRuleID() << ": "
        << Rule::targetToString(rule.getTarget()) << " "
        << rule.getName() << std::endl;
    }

    if (children.empty()) {
      return;
    }

    for (unsigned i = 0; i < children.size() - 1; ++i) {
      std::cout << prefix << "├── ";
      printNode(tree, tree.at(children[i]), prefix + "│   ");
    }

    std::cout << prefix << "└── ";
    printNode(tree, tree.at(children.back()), prefix + "    ");
  }

  /**
   * @brief Recursively prints the rule tree
   *
   * @param tree Rules organized into a tree structure
   */
  static void printTree(const std::map<std::string, std::pair<Rule, std::vector<std::string>>>& tree)
  {
    std::cout << "." << std::endl;
    std::vector<std::pair<Rule, std::vector<std::string>>> roots;

    for (const auto& it : tree) {
      if (!it.second.first) {
        roots.push_back(it.second);
      }
    }

    for (const auto& root : roots) {
      auto children = root.second;

      for (const auto& child : children) {
        if (root == roots.back() && child == children.back()) {
          std::cout << "└── ";
          printNode(tree, tree.at(child), "    ");
        }
        else {
          std::cout << "├── ";
          printNode(tree, tree.at(child), "│   ");
        }
      }
    }
  }

  /**
   * @brief Prints list of devices in a tree format
   *
   * Complexity O(n*log(n)), where n = rules.size
   *
   * @param rules Device rules to print
   */
  static void treeFormat(const std::vector<Rule>& rules)
  {
    /*
     * key: hash
     * value: (rule, children_hashes)
     */
    std::map<std::string, std::pair<Rule, std::vector<std::string>>> tree;

    for (const auto& rule : rules) {
      auto hash = rule.getHash();
      auto p_hash = rule.getParentHash();
      auto hash_it = tree.find(hash);
      auto p_hash_it = tree.find(p_hash);

      if (p_hash_it == tree.end()) {
        tree.insert({p_hash, {{}, {hash}}});
      }
      else {
        p_hash_it->second.second.push_back(hash);
      }

      if (hash_it == tree.end()) {
        tree.insert({hash, {rule, {}}});
      }
      else {
        hash_it->second.first = rule;
      }
    }

    printTree(tree);
  }

  int usbguard_list_devices(int argc, char* argv[])
  {
    bool list_blocked = false;
    bool list_allowed = false;
    bool tree_format = false;
    int opt = 0;

    while ((opt = getopt_long(argc, argv, options_short, options_long, nullptr)) != -1) {
      switch (opt) {
      case 'h':
        showHelp(std::cout);
        return EXIT_SUCCESS;

      case 'a':
        list_allowed = true;
        break;

      case 'b':
        list_blocked = true;
        break;

      case 't':
        tree_format = true;
        break;

      case '?':
        showHelp(std::cerr);

      default:
        return EXIT_FAILURE;
      }
    }

    std::string query = "match";

    if (list_blocked != list_allowed) {
      query = list_allowed ? "allow" : "block";
    }

    usbguard::IPCClient ipc(/*connected=*/true);
    auto device_rules = ipc.listDevices(query);

    if (tree_format) {
      treeFormat(device_rules);
    }
    else {
      classicFormat(device_rules);
    }

    return EXIT_SUCCESS;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
