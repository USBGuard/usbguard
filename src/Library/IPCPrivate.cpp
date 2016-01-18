#include "IPCPrivate.hpp"

namespace usbguard
{
  json IPCPrivate::IPCExceptionToJSON(const IPCException& ex)
  {
    json object = {
      { "_e", ex.codeAsString() },
      { "_i", ex.requestID() },
      { "message", ex.what() }
    };
    return object;
  }

  bool IPCPrivate::isExceptionJSON(const json& object)
  {
    return (object.count("_e") == 1);
  }

  IPCException IPCPrivate::jsonToIPCException(const json& object)
  {
    const std::string code_string = object["_e"];
    const IPCException::ReasonCode code = IPCException::codeFromString(code_string);

    if (object.find("message") != object.end()) {
      return IPCException(code, object["message"], object["_i"]);
    }
    else {
      return IPCException(code, "", object["_i"]);
    }
  }
} /* namespace usbguard */
