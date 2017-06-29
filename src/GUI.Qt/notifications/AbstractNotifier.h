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
  AbstractNotifier() {};

  /**
    Send a basic notification.

    @param title The title of the notification.
    @param body The body of the notification..
    @param urgency The urgency level of the notification
    @param id Optional id to give to the notification. Some backend will map 
              the notification by id and will allow overriding a previous notification
              if the id match.
  */
  virtual void notify(const QString title, const QString body, const Notification::Urgency urgency, const uint32_t id) = 0;

  /**
    Send a notification about a device change.

    @param title The title of the notification.
    @param device_rule The rule change that occured.
    @param urgency The urgency level of the notification.
  */
  virtual void notify(const QString& title, const usbguard::Rule& device_rule, const Notification::Urgency urgency) = 0;
};
