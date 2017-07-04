#include "AbstractBackend.h"

#include <QDBusInterface>
#include <QDBusArgument>
#include <QMap>


/*
  Type of the device attributes sent by the usbguard DBus backend.
*/
typedef QMap<QString, QString> Attributes;
Q_DECLARE_METATYPE (Attributes)

/*
  Marshalling of the usbguard::Rule structure for DBus.

  @param argument DBus argument to create from the rule.
  @param rule Rule to convert to a DBus argument.
*/
QDBusArgument &operator<<(QDBusArgument &argument, const usbguard::Rule &rule);

/*
  Demarshalling of the usbguard::Rule structure for DBus.

  @param argument DBus argument to convert to a Rule.
  @param rule Rule to create from the argument.
*/
const QDBusArgument &operator>>(const QDBusArgument &argument, usbguard::Rule &rule);


/*
  DBus backend to talk to the usbguard daemon.

  This backend uses the DBus interface to to all the communication with the backend.
*/
class DBusBackend: public AbstractBackend
{
  Q_OBJECT
public:
  /*
    Create a new DBusBackend.

    @param parent Parent object in the Qt tree.
  */
  explicit DBusBackend(QObject *parent);

  const std::vector<usbguard::Rule> listDevices(const QString query) override;
  const char* type() override;

private:
  /*
   Connection to the DBus interface.
  */
  QDBusInterface dbus_interface;

private slots:
  /*
    Function called when the backend notifies a change in the presence of a device.

    @param id Id of the device.
    @param event Type of the event that happened.
    @param target Related device information.
    @param device_rule rule that was applied for the given device.
  */
  void devicePresenceChangedSlot(uint id, uint event, uint target, QString device_rule, Attributes attributes);
};
