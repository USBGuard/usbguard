#include "DBusBackend.h"

#include <QtDBus/QtDBus>


DBusBackend::DBusBackend(QObject *parent) :
  AbstractBackend(parent)
{
  qDBusRegisterMetaType<QMap<QString, QString>>();

  if (!QDBusConnection::sessionBus().isConnected()) {
    qFatal("Cannot connect to the D-Bus session bus.\n"
           "To start it, run:\n"
           "\teval `dbus-launch --auto-syntax`");
  }

  // FIXME: handle errors
  const bool connected = QDBusConnection::systemBus().connect("org.usbguard",
                                        "/org/usbguard/Devices",
                                        "org.usbguard.Devices",
                                        "DevicePresenceChanged",
                                        this,
                                        SLOT(devicePresenceChangedSlot(uint, uint, uint, QString, StringDict)));

}


void DBusBackend::devicePresenceChangedSlot(uint id, uint event, uint target, const QString device_rule_string, StringDict /*attributes*/)
{
  emit uiDevicePresenceChange(
    id,
    static_cast<usbguard::DeviceManager::EventType>(event),
    static_cast<usbguard::Rule::Target>(target),
    device_rule_string.toStdString()
  );
}
