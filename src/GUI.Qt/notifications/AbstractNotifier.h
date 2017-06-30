#include "DeviceDialog.h"

#include <QObject>

namespace Notification {
  /**
    Available urgency level for the notifications
  */
  enum Urgency { Information, Warning };
}


/**
  Base Notifier API to background notifications
*/
class AbstractNotifier: protected QObject
{
public:
  /**
    Create a new Abstract notifier
  */
  explicit AbstractNotifier(QObject* parent): QObject(parent) {};

  /**
    Send a basic notification.

    @param title The title of the notification.
    @param body The body of the notification..
    @param urgency The urgency level of the notification
  */
  virtual void notify(const QString title, const QString body, const Notification::Urgency urgency) = 0;

  /**
    Send a notification about a device change.

    @param title The title of the notification.
    @param device_rule The rule change that occured.
    @param urgency The urgency level of the notification.
  */
  virtual void notify(const QString& title, const usbguard::Rule& device_rule, const Notification::Urgency urgency) = 0;
};
