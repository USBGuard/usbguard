//
// Copyright (C) 2016 Red Hat, Inc.
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

#ifdef HAVE_GNU_BASENAME
  /* GNU version of basename(3) */
  #ifndef _GNU_SOURCE
    #define _GNU_SOURCE
  #endif
  #include <cstring>
#else
  /* POSIX version of basename(3) */
  #include <libgen.h>
#endif

#include <cstdlib>  // e.g. for free(3)
#include <cstring>  // e.g. for strdup(3)
#include <iostream>
#include <getopt.h>
#include "DBusBridge.hpp"

static usbguard::DBusBridge* dbus_bridge = nullptr;
static GMainLoop* main_loop = nullptr;
static GDBusNodeInfo* introspection_data = NULL;

static const gchar introspection_xml[] =
#include "DBusInterface.xml.cstr"
  ;
static const unsigned int expected_interface_count = 3;

static int global_ret = EXIT_SUCCESS;

static void
handle_method_call (GDBusConnection*       connection,
  const gchar*           sender,
  const gchar*           object_path,
  const gchar*           interface_name,
  const gchar*           method_name,
  GVariant*              parameters,
  GDBusMethodInvocation* invocation,
  gpointer               user_data)
{
  (void)connection;
  (void)sender;
  (void)object_path;
  (void)user_data;

  try {
    dbus_bridge->handleMethodCall(interface_name, method_name, parameters, invocation);
  }
  catch (std::exception& ex) {
    g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
      G_DBUS_ERROR_FAILED, "Exception: %s", ex.what());
  }
  catch (...) {
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
    global_ret = EXIT_FAILURE;
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
    catch (...) {
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
      std::cerr << "Unable to setup the IPC reconnection timer." << std::endl;
      g_main_loop_quit(main_loop);
      global_ret = EXIT_FAILURE;
      return;
    }
  }
}

static const GDBusInterfaceVTable devices_interface_vtable = {
  handle_method_call,
  nullptr,
  nullptr,
  {}
};

static const GDBusInterfaceVTable policy_interface_vtable = {
  handle_method_call,
  nullptr,
  nullptr,
  {}
};

static const GDBusInterfaceVTable usbguard_interface_vtable = {
  handle_method_call,
  nullptr,
  nullptr,
  {}
};

static void
on_bus_acquired (GDBusConnection* connection,
  const gchar*     name,
  gpointer         user_data)
{
  (void)name;
  (void)user_data;
  auto usbguard_rid = g_dbus_connection_register_object(connection,
      DBUS_ROOT_PATH,
      introspection_data->interfaces[0],
      &usbguard_interface_vtable,
      /*user_data=*/dbus_bridge,
      /*user_data_free_func=*/nullptr,
      /*GError=*/nullptr);
  auto policy_rid = g_dbus_connection_register_object(connection,
      DBUS_POLICY_PATH,
      introspection_data->interfaces[1],
      &policy_interface_vtable,
      /*user_data=*/dbus_bridge,
      /*user_data_free_func=*/nullptr,
      /*GError=*/nullptr);
  auto devices_rid = g_dbus_connection_register_object(connection,
      DBUS_DEVICES_PATH,
      introspection_data->interfaces[2],
      &devices_interface_vtable,
      /*user_data=*/dbus_bridge,
      /*user_data_free_func=*/nullptr,
      /*GError=*/nullptr);

  if (policy_rid <= 0 || devices_rid <= 0 || usbguard_rid <= 0) {
    std::cerr << "Unable to register required objects on the bus." << std::endl;
    g_main_loop_quit(main_loop);
    global_ret = EXIT_FAILURE;
  }
}

static void
on_name_acquired (GDBusConnection* connection,
  const gchar*     name,
  gpointer         user_data)
{
  (void)name;
  (void)user_data;
  /* We got the name, reset the global return value */
  global_ret = EXIT_SUCCESS;

  try {
    dbus_bridge = new usbguard::DBusBridge(connection);
    handle_usbguard_ipc_state(/*state=*/false);
  }
  catch (...) {
    dbus_bridge = nullptr;
    std::cerr << "Unable to create the USBGuard DBus Bridge." << std::endl;
    g_main_loop_quit(main_loop);
    global_ret = EXIT_FAILURE;
  }
}

static void
on_name_lost (GDBusConnection* connection,
  const gchar*     name,
  gpointer         user_data)
{
  (void)connection;
  (void)name;
  (void)user_data;
  g_main_loop_quit(main_loop);
  auto dbus_bridge_local = dbus_bridge;
  dbus_bridge = nullptr;
  delete dbus_bridge_local;
}

static const char* options_short = "sSh";
static const char* usbguard_arg0 = nullptr;

static const struct ::option options_long[] = {
  { "system", no_argument, nullptr, 's' },
  { "session", no_argument, nullptr, 'S' },
  { "help", no_argument, nullptr, 'h' },
  { nullptr, 0, nullptr, 0 }
};

static void showHelp(std::ostream& stream)
{
  char* const writeable_arg0 = ::strdup(usbguard_arg0);
  stream << " Usage: " << ::basename(writeable_arg0) << " [OPTIONS]" << std::endl;
  stream << std::endl;
  stream << " Options:" << std::endl;
  stream << "  -s, --system   Listen on the system bus." << std::endl;
  stream << "  -S, --session  Listen on the session bus." << std::endl;
  stream << "  -h, --help     Show this help." << std::endl;
  stream << std::endl;
  ::free(writeable_arg0);
}

int
main (int argc, char* argv[])
{
  usbguard_arg0 = argv[0];
  int opt = 0;
  bool use_system_bus = true;

  while ((opt = getopt_long(argc, argv, options_short, options_long, nullptr)) != -1) {
    switch (opt) {
    case 's':
      use_system_bus = true;
      break;

    case 'S':
      use_system_bus = false;
      break;

    case 'h':
      showHelp(std::cout);
      return EXIT_SUCCESS;

    case '?':
      showHelp(std::cerr);

    default:
      return EXIT_FAILURE;
    }
  }

  /* Parse the XML DBus interface definition */
  if ((introspection_data = g_dbus_node_info_new_for_xml (introspection_xml, nullptr)) == nullptr) {
    std::cerr << "Failed to parse the introspection data." << std::endl;
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
    std::cerr << "The introspection data contains an unexpected"
      << " number of interfaces: " << interface_count
      << ", expected: " << expected_interface_count << std::endl;
    return EXIT_FAILURE;
  }

  /*
   * Set the global return value to FAILURE before we start the mainloop.
   * When we succesfully own the bus name, this flag will be reset by the
   * DBus callbacks back to SUCCESS. Otherwise, the on_name_lost callback
   * will be called which will stop the main loop and we'll return the
   * FAILURE return code.
   */
  global_ret = EXIT_FAILURE;
  /* Try to take ownership of the bus */
  auto owner_id = g_bus_own_name (use_system_bus ?
      G_BUS_TYPE_SYSTEM : G_BUS_TYPE_SESSION,
      DBUS_SERVICE_NAME,
      G_BUS_NAME_OWNER_FLAGS_NONE,
      on_bus_acquired,
      on_name_acquired,
      on_name_lost,
      /*user_data=*/nullptr,
      /*user_data_free_func=*/nullptr);
  int ret;

  /* Start the main loop */
  if (owner_id > 0) {
    if ((main_loop = g_main_loop_new (NULL, FALSE)) != nullptr) {
      g_main_loop_run (main_loop);
    }

    g_bus_unown_name (owner_id);
    ret = global_ret;
  }
  else {
    std::cerr << "Failed to take ownership of the " << DBUS_SERVICE_NAME << " bus name." << std::endl;
    ret = EXIT_FAILURE;
  }

  /* Release allocated resources */
  g_dbus_node_info_unref (introspection_data);
  return ret;
}

/* vim: set ts=2 sw=2 et */
