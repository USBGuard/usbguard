#ifndef USBFW_DAEMON_HPP
#define USBFW_DAEMON_HPP

#include "Typedefs.hpp"
#include "ConfigFile.hpp"
#include "IPC.hpp"
#include "RuleSet.hpp"
#include "Rule.hpp"

#include "Common/Thread.hpp"
#include "Common/JSON.hpp"

#include "SysIO.hpp"
#include "UDev.hpp"

#include <mutex>
#include <qb/qbipcs.h>
#include <qb/qbloop.h>

namespace usbguard
{
  class Daemon : public IPC
  {
  public:
    Daemon();
    ~Daemon();

    void loadConfiguration(const String& path);
    void loadRules(const String& path);

    /* start threads */
    void run();
    /* stop all threads */
    void quit();

    uint32_t appendRule(const std::string& rule_spec, uint32_t parent_seqn, uint32_t timeout_sec);
    void removeRule(uint32_t seqn);
    void allowDevice(uint32_t seqn, bool append,  uint32_t timeout_sec);
    void blockDevice(uint32_t seqn, bool append, uint32_t timeout_sec);
    void rejectDevice(uint32_t seqn, bool append, uint32_t timeout_sec);
 
    void DeviceInserted(uint32_t seqn,
		     const std::string& name,
		     const std::string& usb_class,
		     const std::string& vendor_id,
		     const std::string& product_id,
		     bool rule_match,
		     uint32_t rule_seqn);

    void DeviceRemoved(uint32_t seqn,
		       const std::string& name,
		       const std::string& usb_class,
		       const std::string& vendor_id,
		       const std::string& product_id);

    void DeviceAllowed(uint32_t seqn,
		       const std::string& name,
		       const std::string& usb_class,
		       const std::string& vendor_id,
		       const std::string& product_id,
		       bool rule_match,
		       uint32_t rule_seqn);

    void DeviceBlocked(uint32_t seqn,
		       const std::string& name,
		       const std::string& usb_class,
		       const std::string& vendor_id,
		       const std::string& product_id,
		       bool rule_match,
		       uint32_t rule_seqn);

    void DeviceRejected(uint32_t seqn,
			const std::string& name,
			const std::string& usb_class,
			const std::string& vendor_id,
			const std::string& product_id,
			bool rule_match,
			uint32_t rule_seqn);

    json processJSON(const json& jobj);
    json processMethodCallJSON(const json& jobj);

  protected:
    static void qbIPCSendJSON(qb_ipcs_connection_t *qb_conn, const json& jobj);
    static int32_t qbSignalHandlerFn(int32_t signal, void *arg);
    static int32_t qbUDevEventFn(int32_t fd, int32_t revents, void *arg);
    static int32_t qbIPCConnectionAcceptFn(qb_ipcs_connection_t *, uid_t, gid_t);
    static void qbIPCConnectionCreatedFn(qb_ipcs_connection_t *);
    static void qbIPCConnectionDestroyedFn(qb_ipcs_connection_t *);
    static int32_t qbIPCConnectionClosedFn(qb_ipcs_connection_t *);
    static int32_t qbIPCMessageProcessFn(qb_ipcs_connection_t *, void *, size_t);
    static int32_t qbIPCJobAdd(enum qb_loop_priority p, void *data, qb_loop_job_dispatch_fn fn);
    static int32_t qbIPCDispatchAdd(enum qb_loop_priority p, int32_t fd, int32_t evts, void *data, qb_ipcs_dispatch_fn_t fn);
    static int32_t qbIPCDispatchMod(enum qb_loop_priority p, int32_t fd, int32_t evts, void *data, qb_ipcs_dispatch_fn_t fn);
    static int32_t qbIPCDispatchDel(int32_t fd);
    static int32_t qbIPCEventFd(int32_t fd, int32_t revents, void *data);

    void initIPC();
    void finiIPC();

    void qbIPCBroadcastData(const struct iovec *iov, size_t iov_len);
    void qbIPCBroadcastString(const std::string& s);

    void udevHandleEvent();
    void processDeviceInsertion(struct udev_device *device);
    void processDeviceRemoval(struct udev_device *device);

    Pointer<const Rule> syncDeviceRule(Pointer<Rule> device_rule);
    void evalDeviceRule(Pointer<Rule> device_rule,
			Pointer<const Rule> matching_rule = makePointer<Rule>());
    void sysioSyncState(Pointer<Rule> device_rule);
    void applyDevicePolicy(uint32_t seqn, Rule::Target target, bool append, uint32_t timeout_sec);

    unsigned int dmAddDeviceRule(Pointer<Rule> rule);
    Pointer<Rule> dmGetDeviceRuleBySeqnMutable(unsigned int seqn);
    Pointer<const Rule> dmGetDeviceRuleBySeqn(unsigned int seqn) const;
    Pointer<const Rule> dmGetDeviceRuleByPath(const String& syspath);
    void dmRemoveDeviceRule(Pointer<const Rule> rule);
    void sysioSetAuthorizedDefault(bool state);

  private:
    ConfigFile _config;
    RuleSet _ruleset;
    qb_loop_t *_qb_loop;
    qb_ipcs_service_t *_qb_service;
    struct udev *_udev;
    struct udev_monitor *_umon;
    PointerMap<unsigned int, Rule> _device_map;
  };
} /* namespace usbguard */

#endif /* USBFW_DAEMON_HPP */
