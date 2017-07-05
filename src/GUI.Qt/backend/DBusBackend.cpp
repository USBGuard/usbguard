#include "DBusBackend.h"

#include <QtDBus/QtDBus>


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
  dbus_interface("org.usbguard", "/org/usbguard", "org.usbguard.Devices", QDBusConnection::systemBus())
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
                                        SLOT(devicePresenceChangedSlot(uint, uint, uint, QString, Attributes)));
}

const char* DBusBackend::type()
{
  return "DBus";
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
      throw usbguard::Exception("listDevices", result.errorName().toStdString(), result.errorMessage().toStdString());
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
      throw usbguard::Exception("applyDevicePolicy", result.errorName().toStdString(), result.errorMessage().toStdString());
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
