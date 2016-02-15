#pragma once

#include <QStringList>
#include <QString>
#include <QList>
#include <QTableWidget>
#include <Rule.hpp>

class DeviceTableWidget : public QTableWidget
{
  Q_OBJECT
public:
  explicit DeviceTableWidget(QWidget * parent = 0);
  ~DeviceTableWidget();

  void setVisibleAttributes(const QStringList& names);

  void insertDevice(const usbguard::Rule& device_rule);
  void updateDevice(uint32_t i, const usbguard::Rule& device_rule);
  void deleteDevice(uint32_t i);

  const QList<uint32_t> getSelectedDeviceIDs() const;
  usbguard::Rule::Target getDeviceTarget(uint32_t id) const;
  void reset();

protected:
  void insertRow(int i, const usbguard::Rule& rule);
  void updateRow(int i, const usbguard::Rule& rule);
  void deleteRow(int i);

private:
  QStringList _visible_attributes;
  QList<uint32_t> _device_id_list;
};

