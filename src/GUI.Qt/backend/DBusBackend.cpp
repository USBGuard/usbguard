#include "DBusBackend.h"

#include <QtDBus/QtDBus>
#include <unistd.h>


Q_DECLARE_METATYPE(usbguard::Rule)


QDBusArgument &operator<<(QDBusArgument &argument, const usbguard::Rule &mystruct)
{
  argument.beginStructure();
  argument << mystruct.getRuleID() << QString::fromStdString(mystruct.toString());
  argument.endStructure();
  return argument;
}


const QDBusArgument &operator>>(const QDBusArgument &argument, usbguard::Rule &mystruct)
{
  QString s;
  uint t;
  argument.beginStructure();
  argument >> t >> s;
  argument.endStructure();

  mystruct = usbguard::Rule::fromString(s.toStdString());
  mystruct.setRuleID(t);
  return argument;
}


DBusBackend::DBusBackend(QObject *parent) :
  AbstractBackend(parent),
  dbus_interface("org.usbguard", "/org/usbguard", "org.usbguard.Devices", QDBusConnection::systemBus()),
  watcher("org.usbguard", QDBusConnection::systemBus(), QDBusServiceWatcher::WatchForOwnerChange, this),
  setup(false)
{
  qDBusRegisterMetaType<QMap<QString, QString>>();

  QObject::connect(&watcher, SIGNAL(serviceRegistered(const QString &)),
                   this, SLOT(_DBusServiceRegisteredSlot(const QString &)));

  QObject::connect(&watcher, SIGNAL(serviceUnregistered(const QString &)),
                   this, SLOT(_DBusServiceUnRegisteredSlot(const QString &)));

  const bool presenceConnected = QDBusConnection::systemBus().connect(
    "org.usbguard",
    "/org/usbguard/Devices",
    "org.usbguard.Devices",
    "DevicePresenceChanged",
    this,
    SLOT(devicePresenceChangedSlot(uint, uint, uint, QString, Attributes)));

  const bool policyConnected = QDBusConnection::systemBus().connect(
    "org.usbguard",
    "/org/usbguard/Devices",
    "org.usbguard.Devices",
    "DevicePolicyChanged",
    this,
    SLOT(devicePolicyChangedSlot(uint, uint, uint, QString, uint)));

  connected = presenceConnected && policyConnected;
}


const char* DBusBackend::type()
{
  return "DBus";
}


bool DBusBackend::isConnected()
{
  return connected;
}


void DBusBackend::connect() {
  if (!setup) {
    if (!QDBusConnection::sessionBus().isConnected()) {
      qFatal("Cannot connect to the D-Bus session bus.\n"
            "To start it, run:\n"
            "\teval `dbus-launch --auto-syntax`");
    }

    setup = true;
    _DBusServiceRegisteredSlot("");
  }
}


void DBusBackend::devicePresenceChangedSlot(uint id, uint event, uint target, const QString device_rule_string, Attributes /*attributes*/)
{
  emit uiDevicePresenceChange(
    id,
    static_cast<usbguard::DeviceManager::EventType>(event),
    static_cast<usbguard::Rule::Target>(target),
    device_rule_string.toStdString()
  );
}


void DBusBackend::devicePolicyChangedSlot(uint id, uint target_old, uint target_new, QString device_rule_string, uint rule_id) {
  emit uiDevicePolicyChanged(
    id,
    static_cast<usbguard::Rule::Target>(target_old),
    static_cast<usbguard::Rule::Target>(target_new),
    device_rule_string.toStdString(),
    rule_id
  );
}


const std::vector<usbguard::Rule> DBusBackend::listDevices(const QString query)
{
  QDBusMessage result = dbus_interface.call("listDevices", query);

  switch (result.type()) {
    case QDBusMessage::ReplyMessage: {
      QDBusArgument devices = result.arguments()[0].value<QDBusArgument>();
      QVector<usbguard::Rule> rules;

      devices >> rules;
      return rules.toStdVector();
    }
    case QDBusMessage::ErrorMessage:
      throw usbguard::Exception("DBusBackend::listDevices", result.errorName().toStdString(), result.errorMessage().toStdString());
    case QDBusMessage::InvalidMessage:
    case QDBusMessage::MethodCallMessage:
    case QDBusMessage::SignalMessage:
    default:
      qFatal("Got a %d while calling DBusBackend::listDevices with error %s",
             result.type(),
             result.errorMessage().toStdString().c_str());
  }
}


void DBusBackend::applyDevicePolicy(uint id, usbguard::Rule::Target target, bool permanent)
{
  QList<QVariant> args;
  args << id << static_cast<uint>(target) << permanent;
  QDBusMessage result = dbus_interface.call("applyDevicePolicy", id, static_cast<uint>(target), permanent);

  switch (result.type()) {
    case QDBusMessage::ReplyMessage:
      break;
    case QDBusMessage::ErrorMessage: {
      throw usbguard::Exception("DBusBackend::applyDevicePolicy", result.errorName().toStdString(), result.errorMessage().toStdString());
    }
    case QDBusMessage::InvalidMessage:
    case QDBusMessage::MethodCallMessage:
    case QDBusMessage::SignalMessage:
    default:
      qFatal("Got a %d while calling DBusBackend::applyDevicePolicy with error %s",
             result.type(),
             result.errorMessage().toStdString().c_str());
  }
}


void DBusBackend::_DBusServiceRegisteredSlot(const QString &) {
  // FIXME: this is a workaround because the dbus backend might
  // not be connected to the daemon yet if we start it after the applet.
  usleep(2000000);
  emit backendConnected();
}


void DBusBackend::_DBusServiceUnRegisteredSlot(const QString &) {
  emit backendDisconnected();
}
