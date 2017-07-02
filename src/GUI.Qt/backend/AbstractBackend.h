#include "usbguard/Interface.hpp"

#include <QObject>


class AbstractBackend: public QObject
{
  Q_OBJECT
public:
  explicit AbstractBackend(QObject* parent): QObject(parent) {};

signals:
  void uiDevicePresenceChange(quint32 id,
                              usbguard::DeviceManager::EventType event,
                              usbguard::Rule::Target target,
                              const std::string& device_rule);
};
