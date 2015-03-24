#include "IPCServer.hpp"
#include "Common/Logging.hpp"
#include <cstring>
#include <sys/eventfd.h>
#include <sys/poll.h>

namespace usbguard
{
  IPCServer::IPCServer()
  {
    static struct qb_ipcs_service_handlers service_handlers = {
      IPCServer::qbConnectionAcceptFn,
      IPCServer::qbConnectionCreatedFn,
      IPCServer::qbMessageProcessFn,
      IPCServer::qbConnectionClosedFn,
      IPCServer::qbConnectionDestroyedFn
    };

    _qb_service = qb_ipcs_create("usbguard", 0,
				 QB_IPC_NATIVE, &service_handlers);

    if (_qb_service == nullptr) {
      throw std::runtime_error("Cannot create qb_service object");
    }

    static struct qb_ipcs_poll_handlers poll_handlers = {
      IPCServer::qbJobAdd,
      IPCServer::qbDispatchAdd,
      IPCServer::qbDispatchMod,
      IPCServer::qbDispatchDel
    };

    qb_ipcs_poll_handlers_set(_qb_service, &poll_handlers);

    return;
  }

  IPCServer::~IPCServer()
  {
    qb_ipcs_destroy(_qb_service);
    _qb_service = nullptr;
    return;
  }

  void IPCServer::start()
  {
    auto rc = qb_ipcs_run(_qb_service);
    if (rc != 0) {
      log->error("Cannot start the IPC server: qb_ipcs_run failed: {}", strerror((int)-rc));
      throw std::runtime_error("IPC server error");
    }
    return;
  }

  int32_t IPCServer::qbConnectionAcceptFn(qb_ipcs_connection_t *conn, uid_t uid, gid_t gid)
  {
    return 0;
  }

  void IPCServer::qbConnectionCreatedFn(qb_ipcs_connection_t *conn)
  {
  }

  void IPCServer::qbConnectionDestroyedFn(qb_ipcs_connection_t *conn)
  {
  }

  int32_t IPCServer::qbConnectionClosedFn(qb_ipcs_connection_t *conn)
  {
    return 0;
  }

  int32_t IPCServer::qbMessageProcessFn(qb_ipcs_connection_t *conn, void *data, size_t size)
  {
    return 0;
  }

  int32_t IPCServer::qbJobAdd(enum qb_loop_priority p, void *data, qb_loop_job_dispatch_fn fn)
  {
    return qb_loop_job_add(IPCServer::qbGetGlobalLoop(), p, data, fn);
  }

  int32_t IPCServer::qbDispatchAdd(enum qb_loop_priority p, int32_t fd, int32_t evts,
				   void *data, qb_ipcs_dispatch_fn_t fn)
  {
    return qb_loop_poll_add(IPCServer::qbGetGlobalLoop(), p, fd, evts, data, fn);
  }

  int32_t IPCServer::qbDispatchMod(enum qb_loop_priority p, int32_t fd, int32_t evts,
				   void *data, qb_ipcs_dispatch_fn_t fn)
  {
    return qb_loop_poll_mod(IPCServer::qbGetGlobalLoop(), p, fd, evts, data, fn);
  }

  int32_t IPCServer::qbDispatchDel(int32_t fd)
  {
    return qb_loop_poll_del(IPCServer::qbGetGlobalLoop(), fd);
  }

  void IPCServer::DeviceInserted(uint32_t seqn,
			      const std::string& name,
			      const std::string& usb_class,
			      const std::string& vendor_id,
			      const std::string& product_id,
			      bool rule_match,
			      uint32_t rule_seqn)
  {
    log->debug("DeviceInserted: seqn={}, name={}, usb_class={}, "
	       "vendor_id={}, product_id={}, rule_match={}, rule_seqn={}",
	       seqn, name, usb_class, vendor_id, product_id, rule_match, rule_seqn);
  }

  void IPCServer::DeviceRemoved(uint32_t seqn,
				const std::string& name,
				const std::string& usb_class,
				const std::string& vendor_id,
				const std::string& product_id)
  {
    log->debug("DeviceRemoved: seqn={}, name={}, usb_class={}, "
	       "vendor_id={}, product_id={}",
	       seqn, name, usb_class, vendor_id, product_id);
  }

  void IPCServer::DeviceAllowed(uint32_t seqn,
				const std::string& name,
				const std::string& usb_class,
				const std::string& vendor_id,
				const std::string& product_id,
				bool rule_match,
				uint32_t rule_seqn)
  {
    log->debug("DeviceAllowed: seqn={}, name={}, usb_class={}, "
	       "vendor_id={}, product_id={}, rule_match={}, rule_seqn={}",
	       seqn, name, usb_class, vendor_id, product_id, rule_match, rule_seqn);
  }

  void IPCServer::DeviceBlocked(uint32_t seqn,
				const std::string& name,
				const std::string& usb_class,
				const std::string& vendor_id,
				const std::string& product_id,
				bool rule_match,
				uint32_t rule_seqn)
  {
    log->debug("DeviceBlocked: seqn={}, name={}, usb_class={}, "
	       "vendor_id={}, product_id={}, rule_match={}, rule_seqn={}",
	       seqn, name, usb_class, vendor_id, product_id, rule_match, rule_seqn);
  }

  void IPCServer::DeviceRejected(uint32_t seqn,
				 const std::string& name,
				 const std::string& usb_class,
				 const std::string& vendor_id,
				 const std::string& product_id,
				 bool rule_match,
				 uint32_t rule_seqn)
  {
    log->debug("DeviceRejected: seqn={}, name={}, usb_class={}, "
	       "vendor_id={}, product_id={}, rule_match={}, rule_seqn={}",
	       seqn, name, usb_class, vendor_id, product_id, rule_match, rule_seqn);
  }

  void IPCServer::qbBroadcastData(const struct iovec *iov, size_t iov_len)
  {
    auto qb_conn = qb_ipcs_connection_first_get(_qb_service);
    size_t total_size = 0;

    for (size_t i = 0; i < iov_len; ++i) {
      total_size += iov[i].iov_len;
    }

    while (qb_conn != nullptr) {
      /* Send the data */
      ssize_t rc = qb_ipcs_event_sendv(qb_conn, iov, iov_len);

      if (rc < 0) {
	/* FIXME: There's no client identification value in the message */
	log->warn("Failed to send broadcast data to: {}", strerror((int)-rc));
      }
      else if ((size_t)rc != total_size) {
	/* FIXME: There's no client identification value in the message */
	log->warn("Sent less data than expected to. Expected {}, send {}.",
		  total_size, rc);
      }
      
      /* Get the next connection */
      auto qb_conn_next = qb_ipcs_connection_next_get(_qb_service, qb_conn);
      qb_ipcs_connection_unref(qb_conn);
      qb_conn = qb_conn_next;
    }

    return;
  }

  qb_loop_t *IPCServer::qbGetGlobalLoop()
  {
    if (G_qb_global_loop == nullptr) {
      throw std::runtime_error("qb_global_loop == nullptr");
    }
    return G_qb_global_loop;
  }

} /* namespace usbguard */
