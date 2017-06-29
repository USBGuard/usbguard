#include "DBusNotifier.h"

#include <iostream>
#include <QtCore/QCoreApplication>
#include <QtDBus/QtDBus>
#include <QDBusReply>


DBusNotifier::DBusNotifier() : AbstractNotifier()
{
  QDBusConnection bus = QDBusConnection::sessionBus();

  dbus_interface = new QDBusInterface("org.freedesktop.Notifications", "/org/freedesktop/Notifications", "org.freedesktop.Notifications", bus, this);
  if (!bus.isConnected()) {
    fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                    "To start it, run:\n"
                    "\teval `dbus-launch --auto-syntax`\n");
    exit(1);
  }
}


void DBusNotifier::notify(const QString &title, const usbguard::Rule& device_rule, const Notification::Urgency urgency)
{
  const std::string device_id = device_rule.getDeviceID().toString();
  uint32_t id;

  QMap<std::string, uint32_t>::const_iterator it = current_notifications.find(device_id);

  if (it == current_notifications.end()) {
    id = getNextId();
    current_notifications.insert(device_id, id);
  } else {
    id = it.value();
  }

  const QString usb_id = QString::fromStdString(device_rule.getDeviceID().toString());
  const QString name = QString::fromStdString(device_rule.getName());
  const QString port = QString::fromStdString(device_rule.getViaPort());

  const QString body =
  QString("USB ID: %1\n"
          "Name: %2\n"
          "Port: %3\n")
    .arg(usb_id)
    .arg(name)
    .arg(port);

  notify(title, body, urgency, id);
}


void DBusNotifier::notify(QString title, QString body, Notification::Urgency urgency, uint32_t id)
{
  QMap<QString, QVariant> hints;
  int32_t timeout;

  switch (urgency) {
    case Notification::Urgency::Warning:
      hints.insert("urgency", 2);
      timeout = 5000;
      break;
    case Notification::Urgency::Information:
    default:
      hints.insert("urgency", 1);
      timeout = 2000;
  }

  dbus_interface->call("Notify",
                       /* app name */ "usbguard",
                       /* replace id */ id,
                       /* icon path */ "usbguard-icon",
                       /* title */ title,
                       /* body */ body,
                       /* actions */ QVariant(QList<QString>()),
                       /* hints */ QVariant(hints),
                       /* timeout */ timeout
  );
}

uint32_t DBusNotifier::getNextId()
{
  int i;
  while ((i = nextAvailableId++) == 0) {}
  return i;
}
