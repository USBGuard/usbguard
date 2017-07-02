#include "AbstractBackend.h"

#include <QDBusInterface>
#include <QMap>

typedef QMap<QString, QString> StringDict;
Q_DECLARE_METATYPE (StringDict)


class DBusBackend: public AbstractBackend
{
  Q_OBJECT
public:
  explicit DBusBackend(QObject *parent);

private slots:
  void devicePresenceChangedSlot(uint id, uint event, uint target, QString device_rule, StringDict attributes);
};
