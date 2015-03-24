#pragma once

#include <cstdint>
#include <stdexcept>
#include <sodium.h>
#include <atomic>

namespace usbguard
{
  class IPCException : public std::exception
  {
  public:
    enum ReasonCode {
      InvalidArgument, /**< Incorrect value provided (out-of-range, invalid type, ...) */
      NotFound, /**< No such object found (rule or device) */
      SyscallFailure, /**< An underlying system call failed */
      PermissionDenied, /**< The caller is not allowed to perform the requested action */
      TransientError, /**< The requested action cannot be executed at the moment, try again */
      ProtocolError, /**< An IPC protocol error (missing attribute, return value, incorrect type */
      ConnectionError,
    };

    IPCException(ReasonCode code, const std::string& message)
      : _message(message),
	_code(code)
    {
    }

    const char *what() const noexcept
    {
      return _message.c_str();
    }

    ReasonCode code() const
    {
      return _code;
    }

    const char * const codeAsString() const
    {
      switch(_code) {
      case InvalidArgument:
	return "Invalid Argument";
      case NotFound:
	return "Not Found";
      case SyscallFailure:
	return "Syscall Failure";
      case PermissionDenied:
	return "Permission Denied";
      case TransientError:
	return "Transient Error";
      case ProtocolError:
	return "Protocol Error";
      case ConnectionError:
	return "Connection Error";
      }
      return "";
    }

  private:
    const std::string& _message;
    ReasonCode _code;
  };

  class IPC
  {
  public:
    /* IPC methods */
    virtual uint32_t appendRule(const std::string& rule_spec,
				uint32_t parent_seqn,
				uint32_t timeout_sec) = 0;

    virtual void removeRule(uint32_t seqn) = 0;

    virtual void allowDevice(uint32_t seqn,
			     bool append,
			     uint32_t timeout_sec) = 0;

    virtual void blockDevice(uint32_t seqn,
			     bool append,
			     uint32_t timeout_sec) = 0;

    virtual void rejectDevice(uint32_t seqn,
			      bool append,
			      uint32_t timeout_sec) = 0;

    /* IPC Signals */
    virtual void DeviceInserted(uint32_t seqn,
				const std::string& name,
				const std::string& usb_class,
				const std::string& vendor_id,
				const std::string& product_id,
				bool rule_match,
				uint32_t rule_seqn) = 0;
#if 0
    virtual void DevicePresent(uint32_t seqn,
			       const std::string& name,
			       const std::string& usb_class,
			       const std::string& vendor_id,
			       const std::string& product_id,
			       const std::string& target) = 0;
#endif
    virtual void DeviceRemoved(uint32_t seqn,
			       const std::string& name,
			       const std::string& usb_class,
			       const std::string& vendor_id,
			       const std::string& product_id) = 0;

    virtual void DeviceAllowed(uint32_t seqn,
			       const std::string& name,
			       const std::string& usb_class,
			       const std::string& vendor_id,
			       const std::string& product_id,
			       bool rule_match,
			       uint32_t rule_seqn) = 0;

    virtual void DeviceBlocked(uint32_t seqn,
			       const std::string& name,
			       const std::string& usb_class,
			       const std::string& vendor_id,
			       const std::string& product_id,
			       bool rule_match,
			       uint32_t rule_seqn) = 0;

    virtual void DeviceRejected(uint32_t seqn,
				const std::string& name,
				const std::string& usb_class,
				const std::string& vendor_id,
				const std::string& product_id,
				bool rule_match,
				uint32_t rule_seqn) = 0;

    static uint64_t uniqueID(void)
    {
      static std::atomic<uint64_t> id(0);
      return id++;
    }
  };

} /* namespace usbguard */
