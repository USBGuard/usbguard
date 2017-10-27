//
// Copyright (C) 2017 Red Hat, Inc.
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

#include "LinuxAuditBackend.hpp"

#include <usbguard/Exception.hpp>
#include <usbguard/Logger.hpp>
#include "Common/Utility.hpp"
#include "Library/Utility.hpp"

#include <stdexcept>

#if defined(HAVE_LINUX_AUDIT)
#include <libaudit.h>

#ifndef AUDIT_USER_DEVICE
  #define AUDIT_USER_DEVICE 1137 /* User space hotplug device changes */
#endif

#if !defined(HAVE_DECL_AUDIT_ENCODE_NV_STRING)
#ifndef _GNU_SOURCE
  #define _GNU_SOURCE 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * Audit message encoding functions based on:
 *  https://github.com/linux-audit/audit-userspace/blob/f7cd4d6/lib/audit_logging.c#L125
 */

/*
* This function checks a string to see if it needs encoding.
* It returns true if needed and false if not.
*/
static bool audit_value_needs_encoding(const char* str, unsigned int size)
{
  if (str == NULL) {
    return false;
  }

  for (unsigned int i = 0; i < size; i++) {
    // we don't test for > 0x7f because str[] is signed.
    if (str[i] == '"' || str[i] < 0x21 || str[i] == 0x7F) {
      return true;
    }
  }

  return false;
}

/*
* This function does encoding of "untrusted" names just like the kernel
*/
static char* audit_encode_value(char* final, const char* buf, unsigned int size)
{
  char* ptr = final;
  const char* hex = "0123456789ABCDEF";

  if (final == NULL) {
    return NULL;
  }

  if (buf == NULL) {
    *final = 0;
    return final;
  }

  for (unsigned int i = 0; i < size; i++) {
    *ptr++ = hex[(buf[i] & 0xF0)>>4]; /* Upper nibble */
    *ptr++ = hex[buf[i] & 0x0F];      /* Lower nibble */
  }

  *ptr = 0;
  return final;
}

static char* audit_encode_nv_string(const char* name, const char* value, unsigned int vlen)
{
  char* str = nullptr;

  if (vlen == 0 && value) {
    vlen = ::strlen(value);
  }

  if (value && audit_value_needs_encoding(value, vlen)) {
    char* tmp = reinterpret_cast<char*>(::malloc(2*vlen + 1));

    if (tmp) {
      audit_encode_value(tmp, value, vlen);

      if (::asprintf(&str, "%s=%s", name, tmp) < 0) {
        str = nullptr;
      }

      free(tmp);
    }
    else {
      str = nullptr;
    }
  }
  else {
    if (::asprintf(&str, "%s=\"%s\"", name, value ? value : "?") < 0) {
      str = nullptr;
    }
  }

  return str;
}
#endif /* HAVE_DECL_AUDIT_ENCODE_NV_STRING */
#endif /* HAVE_LINUX_AUDIT */

namespace usbguard
{
#if defined(HAVE_LINUX_AUDIT)
  static std::string encodeMessageNVPair(const std::string& key, const std::string& value)
  {
    std::unique_ptr<char, FreeDeleter> encoded_cstr(audit_encode_nv_string(key.c_str(), value.c_str(), value.size()));
    std::string encoded(encoded_cstr.get());
    return encoded;
  }

  static void appendToMessage(std::string& message, const std::string& key, const std::string& value)
  {
    if (message.size() > 0) {
      message.append(" ");
    }

    message.append(encodeMessageNVPair(key, value));
  }

  static std::string translateTypeValue(const std::string& value)
  {
    if (hasPrefix(value, "Policy.Device.")) {
      /* device policy events */
      if (hasSuffix(value, "Update")) {
        return "changed-authorization-state-for";
      }

      throw std::runtime_error("LinuxAuditBackend: unexpected Policy.Device event");
    }
    else if (hasPrefix(value, "Device.")) {
      /* device events */
      if (hasSuffix(value, "Present")) {
        return "discovered-device";
      }

      if (hasSuffix(value, "Insert")) {
        return "inserted-device";
      }

      if (hasSuffix(value, "Update")) {
        return "updated-device";
      }

      if (hasSuffix(value, "Remove")) {
        return "removed-device";
      }

      throw std::runtime_error("LinuxAuditBackend: unexpected Device event");
    }
    else if (hasPrefix(value, "Policy.")) {
      /* rule set modification events */
      if (hasSuffix(value, "Insert")) {
        return "inserted-policy-rule";
      }

      if (hasSuffix(value, "Update")) {
        return "updated-policy-rule";
      }

      if (hasSuffix(value, "Remove")) {
        return "removed-policy-rule";
      }

      throw std::runtime_error("LinuxAuditBackend: unexpected Policy event");
    }

    throw std::runtime_error("LinuxAuditBackend: unexpected type= key value");
  }
#endif

  LinuxAuditBackend::LinuxAuditBackend()
  {
#if defined(HAVE_LINUX_AUDIT)
    USBGUARD_LOG(Debug) << "Opening Linux Audit socket";

    if ((_audit_fd = audit_open()) < 0) {
      throw ErrnoException("LinuxAuditBackend", "audit_open", errno);
    }

#else
    (void)_audit_fd;
    throw std::runtime_error("LinuxAuditBackend: not supported");
#endif
  }

  LinuxAuditBackend::~LinuxAuditBackend()
  {
#if defined(HAVE_LINUX_AUDIT)
    USBGUARD_LOG(Debug) << "Closing Linux Audit socket";
    audit_close(_audit_fd);
#endif
  }

  void LinuxAuditBackend::write(const AuditEvent& event)
  {
#if defined(HAVE_LINUX_AUDIT)
    std::string message;
    /*
     * Linux Audit event result
     *  0 = failed
     *  1 = success
     */
    int result = 0;
    auto kvals = event.keys();
    std::string audit_key;
    std::string audit_value;

    if (kvals.count("type") > 0) {
      audit_key = "op";
      audit_value = translateTypeValue(kvals["type"]);
      appendToMessage(message, audit_key, audit_value);
      kvals.erase("type");
    }
    else {
      throw std::runtime_error("missing required type field in the audit event");
    }

    if (kvals.count("result") > 0) {
      if (kvals["result"] == "SUCCESS") {
        result = 1;
      }

      kvals.erase("result");
    }
    else {
      throw std::runtime_error("missing required result field in the audit event");
    }

    if (kvals.count("device.system_name") > 0) {
      audit_key = "device";
      audit_value = kvals["device.system_name"];
      appendToMessage(message, audit_key, audit_value);
      kvals.erase("device.system_name");
    }

    if (kvals.count("rule") > 0) {
      audit_key = "rule";
      audit_value = kvals["rule"];
      appendToMessage(message, audit_key, audit_value);
      kvals.erase("rule");
    }

    for (const auto& kv_pair : kvals) {
      const std::string& key = kv_pair.first;
      const std::string& value = kv_pair.second;
      audit_key.clear();
      audit_value.clear();

      if (key == "device.rule") {
        audit_key = "device_rule";
        audit_value = value;
      }
      else if (key == "device.rule.new") {
        audit_key = "device_rule";
        audit_value = value;
      }
      else if (key == "target") {
        audit_key = "target";
        audit_value = value;
      }
      else if (key == "target.new") {
        audit_key = "target";
        audit_value = value;
      }
      else if (key == "rule.id") {
        audit_key = "rule_id";
        audit_value = value;
      }
      else if (key == "rule.new") {
        audit_key = "rule";
        audit_value = value;
      }

      /*
       * Skip appending the key=value pair to the message, if the
       * audit_key string is empty. This is used to skip key=value
       * pairs that we don't want to include in the Linux Audit trail.
       */
      if (!audit_key.empty()) {
        appendToMessage(message, audit_key, audit_value);
      }
    }

    USBGUARD_LOG(Debug) << "Writing Linux Audit message: " << message;
    audit_log_user_message(_audit_fd, AUDIT_USER_DEVICE, message.c_str(),
      /*hostname=*/nullptr, /*addr=*/nullptr, /*tty=*/nullptr, result);
#else
    (void)event;
    throw std::runtime_error("LinuxAuditBackend::write: not supported");
#endif
  }
}
/* vim: set ts=2 sw=2 et */
