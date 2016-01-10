#pragma once

#include "IPC.hpp"
#include "Typedefs.hpp"
#include "Common/JSON.hpp"

namespace usbguard
{
  class DLL_PUBLIC IPCPrivate
  {
  public:
    static bool isExceptionJSON(const json& object);
    static IPCException jsonToIPCException(const json& object);
    static json IPCExceptionToJSON(const IPCException& ex);
  };
} /* namespace usbguard */
