#include "AbstractNotifier.h"

#include <QDBusInterface>


/**
  Notifier using the DBus notification protocole to send notifications to the user.

  For more information about the DBus notification interface, please see
  https://developer.gnome.org/notification-spec/.
*/
class DBusNotifier: AbstractNotifier {
public:
  /**
    Create a new DBus notifier
  */
  DBusNotifier();

  void notify(const QString title, const QString body, const Notification::Urgency urgency, const uint32_t id = 0);
  void notify(const QString& title, const usbguard::Rule& device_rule, const Notification::Urgency urgency);

private:
  // connection to the DBus interface
  QDBusInterface *dbus_interface;
  // map of currently displayed notifications
  QMap<std::string, uint32_t> current_notifications;
  // next available UID for the notifications
  uint32_t nextAvailableId = 0;

  // get the next available id for a notification.
  uint32_t getNextId();
};
