#pragma once
#include <cstdint>
#include <endian.h>

namespace usbguard {

  static inline uint16_t busEndianToHost(uint16_t bus_value)
  {
    return le16toh(bus_value);
  }

} /* namespace usbguard */
