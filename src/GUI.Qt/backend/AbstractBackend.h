#include "usbguard/Interface.hpp"

#include <QObject>


/*
  Base Backend class.

  This class defines the common interface between the different backends available
  to talk to the usbguard-daemon.
*/
class AbstractBackend: public QObject
{
  Q_OBJECT
public:
  /*
    Create a new backend to talk to USBGuard.

    @param parent The parent object in the QT tree.
  */
  explicit AbstractBackend(QObject* parent): QObject(parent) {};

  /*
    List devices currently connected.

    Currently connected devices can be filtered using the query parameter.
    See DBus/DBusInterface.xml for more information about valid values.

    @param query parameter to filter the devices list.
    @return a vector of all currently connected devices.
  */
  // FIXME: Rule is a bit weird as a name that lists devices and their information...
  virtual const std::vector<usbguard::Rule> listDevices(const QString query) = 0;

  /*
    Apply a given rule for the given device id.

    @param id of the device for which to apply the rule.
    @param target Target rule to apply.
    @param permanent Whether this is a permanent rule change or not.
  */
  virtual void applyDevicePolicy(uint id, usbguard::Rule::Target target, bool permanent) = 0;

  /*
    Type of the backend 
  */
  virtual const char* type() = 0;

signals:
  /*
    Signal emitted when a device appears or disappears.

    @param id Id of the device.
    @param event Type of the event that happened.
    @param target Related device information.
    @param device_rule rule that was applied for the given device.
  */
  void uiDevicePresenceChange(quint32 id,
                              usbguard::DeviceManager::EventType event,
                              usbguard::Rule::Target target,
                              const std::string& device_rule);
};
