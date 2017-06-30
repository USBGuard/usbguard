#include "DBusNotifier.h"

#include <QtCore/QCoreApplication>
#include <QtDBus/QtDBus>
#include <QDBusReply>
#include <QApplication>
#include <QDBusMessage>

DBusNotifier::DBusNotifier(QObject *parent) :
  AbstractNotifier(parent),
  dbus_interface("org.freedesktop.Notifications", "/org/freedesktop/Notifications", "org.freedesktop.Notifications")
{
  if (!QDBusConnection::sessionBus().isConnected()) {
    qFatal("Cannot connect to the D-Bus session bus.\n"
           "To start it, run:\n"
           "\teval `dbus-launch --auto-syntax`");
  }

  // TODO: This seems to never fail. We should investigate
  QDBusConnection::sessionBus().connect("org.freedesktop.Notifications",
                                        "/org/freedesktop/Notifications",
                                        "org.freedesktop.Notifications",
                                        "NotificationClosed",
                                        this,
                                        SLOT(notificationClosedSlot(uint)));
}

void DBusNotifier::notify(QString title, QString body, Notification::Urgency urgency)
{
  _notify(title, body, urgency);
}

void DBusNotifier::notify(const QString &title, const usbguard::Rule &device_rule, const Notification::Urgency urgency)
{
  const std::string device_id = device_rule.getDeviceID().toString();

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

  lock.lock();
  const QMap<std::string, uint>::const_iterator it = device_to_notification.find(device_id);

  if (it == device_to_notification.end()) {
    const uint new_id = _notify(title, body, urgency);
    device_to_notification.insert(device_id, new_id);
    notification_to_device.insert(new_id, device_id);
  }
  else {
    _notify(title, body, urgency, it.value());
  }
  lock.unlock();
}

uint DBusNotifier::_notify(QString title, QString body, Notification::Urgency urgency, uint id)
{
  QMap<QString, QVariant> hints;
  int timeout;

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

  QDBusMessage msg = dbus_interface.call("Notify",
                                         /* app name */ "usbguard",
                                         /* replace id */ id,
                                         /* icon path */ "usbguard-icon",
                                         /* title */ title,
                                         /* body */ body,
                                         /* actions */ QVariant(QList<QString>()),
                                         /* hints */ QVariant(hints),
                                         /* timeout */ timeout);

  return msg.arguments().at(0).toUInt();
}

void DBusNotifier::notificationClosedSlot(uint notif_id)
{
  lock.lock();
  const QMap<uint, std::string>::iterator it = notification_to_device.find(notif_id);

  if (it != notification_to_device.end()) {
    device_to_notification.remove(it.value());
    notification_to_device.erase(it);
  }

  lock.unlock();
}
