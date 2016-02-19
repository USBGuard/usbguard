#include <gio/gio.h>
#include <stdlib.h>
#include "DBusBridge.hpp"


static usbguard::DBusBridge * dbus_bridge = nullptr;
static GMainLoop * main_loop = nullptr;
static GDBusNodeInfo *introspection_data = NULL;

static const gchar introspection_xml[] =
#include "DBusInterface.xml.cstr"
;
static const unsigned int expected_interface_count = 2;

static void
handle_method_call (GDBusConnection       *connection,
                    const gchar           *sender,
                    const gchar           *object_path,
                    const gchar           *interface_name,
                    const gchar           *method_name,
                    GVariant              *parameters,
                    GDBusMethodInvocation *invocation,
                    gpointer               user_data)
{
  try {
    dbus_bridge->handleMethodCall(interface_name, method_name, parameters, invocation);
  }
  catch(std::exception& ex) {
    g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
       G_DBUS_ERROR_FAILED, "Exception: %s", ex.what());
  }
  catch(...) {
    g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
       G_DBUS_ERROR_FAILED, "BUG: Unknown exception; method call failed for unknown reasons.");
  }
}

static gboolean
usbguard_ipc_try_connect(gpointer user_data)
{
  (void)user_data;

  if (dbus_bridge == nullptr) {
    g_main_loop_quit(main_loop);
    return FALSE;
  }
  else if (dbus_bridge->isConnected()) {
   /* returning FALSE removes the function call from the main loop */
   return FALSE;
  }
  else {
    try {
      dbus_bridge->connect();
    }
    catch(...) {
      /* ignore exception */
    }
    return TRUE;
  }
}

static void
handle_usbguard_ipc_state(bool state)
{
  if (state == false) {
    if (g_timeout_add_seconds(1, &usbguard_ipc_try_connect, nullptr) <= 0) {
      g_main_loop_quit(main_loop);
      return;
    }
  }
}

static const GDBusInterfaceVTable devices_interface_vtable =
{
  handle_method_call,
  nullptr,
  nullptr
};

static const GDBusInterfaceVTable policy_interface_vtable =
{
  handle_method_call,
  nullptr,
  nullptr
};

static void
on_bus_acquired (GDBusConnection *connection,
                 const gchar     *name,
                 gpointer         user_data)
{
  auto policy_rid = g_dbus_connection_register_object(connection,
                                                      "/org/usbguard/Policy",
                                                      introspection_data->interfaces[0],
                                                      &policy_interface_vtable,
                                                      /*user_data=*/dbus_bridge,
                                                      /*user_data_free_func=*/nullptr,
                                                      /*GError=*/nullptr);
  auto devices_rid = g_dbus_connection_register_object(connection,
                                                      "/org/usbguard/Devices",
                                                      introspection_data->interfaces[1],
                                                      &devices_interface_vtable,
                                                      /*user_data=*/dbus_bridge,
                                                      /*user_data_free_func=*/nullptr,
                                                      /*GError=*/nullptr);
  if (policy_rid <= 0 || devices_rid <= 0) {
    g_main_loop_quit(main_loop);
  }
}

static void
on_name_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
  try {
    dbus_bridge = new usbguard::DBusBridge(connection);
    handle_usbguard_ipc_state(/*state=*/false);
  }
  catch(...) {
    dbus_bridge = nullptr;
    g_main_loop_quit(main_loop);
  }
}

static void
on_name_lost (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
  g_main_loop_quit(main_loop);
  auto dbus_bridge_local = dbus_bridge;
  dbus_bridge = nullptr;
  delete dbus_bridge_local;
}

int
main (int argc, char *argv[])
{
  /* Parse the XML DBus interface definition */
  if ((introspection_data = g_dbus_node_info_new_for_xml (introspection_xml, nullptr)) == nullptr) {
    return EXIT_FAILURE;
  }

  /* Ensure that the expected number of interfaces is defined */
  unsigned int interface_count = 0;
  if (introspection_data->interfaces != nullptr) {
    while (introspection_data->interfaces[interface_count] != nullptr) {
      ++interface_count;
    }
  }
  if (interface_count != expected_interface_count) {
    return EXIT_FAILURE;
  }

  /* Try to take ownership of the bus */
  auto owner_id = g_bus_own_name (G_BUS_TYPE_SESSION,
                                 "org.usbguard",
                                 G_BUS_NAME_OWNER_FLAGS_NONE,
                                 on_bus_acquired,
                                 on_name_acquired,
                                 on_name_lost,
                                 /*user_data=*/nullptr,
                                 /*user_data_free_func=*/nullptr);

  /* Start the main loop */
  if (owner_id > 0) {
    if ((main_loop = g_main_loop_new (NULL, FALSE)) != nullptr) {
      g_main_loop_run (main_loop);
    }
    g_bus_unown_name (owner_id);
  }

  /* Release allocated resources */
  g_dbus_node_info_unref (introspection_data);

  return 0;
}

