#pragma once

#include "Common/IPC.hpp"
#include "Common/Thread.hpp"
#include <qb/qbipcs.h>
#include <qb/qbloop.h>

namespace usbguard
{
  class IPCServer : public IPC
  {
  public:
    IPCServer();
    virtual ~IPCServer();

    void start();
    void stop();

    virtual uint32_t appendRule(const std::string& rule_spec,
				uint32_t parent_seqn,
				uint32_t timeout_sec) = 0;

    virtual void removeRule(uint32_t seqn) = 0;

    virtual void allowDevice(uint32_t seqn,
			     bool append,
			     uint32_t timeout_sec) = 0;

    virtual void denyDevice(uint32_t seqn,
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
			     uint32_t rule_seqn);

    virtual void DeviceRemoved(uint32_t seqn,
			       const std::string& name,
			       const std::string& usb_class,
			       const std::string& vendor_id,
			       const std::string& product_id);

    virtual void DeviceAllowed(uint32_t seqn,
			       const std::string& name,
			       const std::string& usb_class,
			       const std::string& vendor_id,
			       const std::string& product_id,
			       bool rule_match,
			       uint32_t rule_seqn);

    virtual void DeviceDenied(uint32_t seqn,
			      const std::string& name,
			      const std::string& usb_class,
			      const std::string& vendor_id,
			      const std::string& product_id,
			      bool rule_match,
			      uint32_t rule_seqn);

    virtual void DeviceRejected(uint32_t seqn,
				const std::string& name,
				const std::string& usb_class,
				const std::string& vendor_id,
				const std::string& product_id,
				bool rule_match,
				uint32_t rule_seqn);
  protected:
    static int32_t qbConnectionAcceptFn(qb_ipcs_connection_t *, uid_t, gid_t);
    static void qbConnectionCreatedFn(qb_ipcs_connection_t *);
    static void qbConnectionDestroyedFn(qb_ipcs_connection_t *);
    static int32_t qbConnectionClosedFn(qb_ipcs_connection_t *);
    static int32_t qbMessageProcessFn(qb_ipcs_connection_t *, void *, size_t);
    static int32_t qbJobAdd(enum qb_loop_priority p, void *data, qb_loop_job_dispatch_fn fn);
    static int32_t qbDispatchAdd(enum qb_loop_priority p, int32_t fd, int32_t evts, void *data, qb_ipcs_dispatch_fn_t fn);
    static int32_t qbDispatchMod(enum qb_loop_priority p, int32_t fd, int32_t evts, void *data, qb_ipcs_dispatch_fn_t fn);
    static int32_t qbDispatchDel(int32_t fd);
    static int32_t qbEventFd(int32_t fd, int32_t revents, void *data);

    void thread();
    void qbBroadcastData(const struct iovec *iov, size_t iov_len);

    static qb_loop_t *qbGetGlobalLoop();

  private:
    int _eventfd;
    Thread<IPCServer> _thread;
    qb_loop_t *_qb_loop;
    qb_ipcs_service_t *_qb_service;
  };

  extern qb_loop_t *G_qb_global_loop;

} /* namespace usbguard */
