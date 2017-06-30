#include "AbstractNotifier.h"

#include <QDBusInterface>
#include <QMutex>


/**
  Notifier using the DBus notification protocole to send notifications to the user.

  For more information about the DBus notification interface, please see
  https://developer.gnome.org/notification-spec/.
*/
class DBusNotifier: AbstractNotifier {
  Q_OBJECT
public:
  /**
    Create a new DBus notifier
  */
  explicit DBusNotifier(QObject *parent);

  void notify(const QString title, const QString body, const Notification::Urgency urgency);
  void notify(const QString& title, const usbguard::Rule& device_rule, const Notification::Urgency urgency);

private:
  // connection to the DBus interface
  QDBusInterface dbus_interface;

  // map of devices to currently displayed notifications
  QMap<std::string, uint> device_to_notification;

  // map of currently displayed notifications to devices
  QMap<uint, std::string> notification_to_device;

  // mutex to protect modification of both maps due to asynchrony with slots
  QMutex lock;

  // internal version of notification to handle ids that DBus notifications implement.
  uint _notify(const QString title, const QString body, const Notification::Urgency urgency, uint id = 0);

private slots:
  // slot for when a notification is closed, to do some cleaning on the server
  void notificationClosedSlot(uint id);
};
