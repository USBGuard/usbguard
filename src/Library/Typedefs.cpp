#include "Typedefs.hpp"

namespace usbguard {

  template<>
  bool matches(const String& a, const String& b)
  {
    return a == b;
  }

} /* namespace usbguard */
