#include <iostream>
#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif
#include <cstring>
#include "Rule.hpp"
#include "RuleParser.hpp"

int main(int argc, char **argv)
{
  if (argc != 2) {
    std::cerr << "Usage: " << ::basename(argv[0]) << " <rulespec>" << std::endl;
  }
  else {
    try {
      const std::string rule_spec(argv[1]);
      std::cout << "INPUT: " << rule_spec << std::endl;
      const usbguard::Rule rule = usbguard::parseRuleSpecification(rule_spec);
      std::cout << "OUTPUT: " << rule.toString() << std::endl;
      return EXIT_SUCCESS;
    }
    catch(const usbguard::RuleParserError& ex) {
      std::cerr << "ERROR: " << ex.what() << std::endl;
      std::cerr << "HINT: " << ex.hint() << std::endl;
      if (ex.hasFileInfo()) {
        std::cerr << "FILE: " << ex.fileInfo() << std::endl;
      }
    }
    catch(const std::exception& ex) {
      std::cerr << "EXCEPTION: " << ex.what() << std::endl;
    }
    catch(...) {
      std::cerr << "EXCEPTION: Unknown" << std::endl;
    }
  }
  return EXIT_FAILURE;
}
