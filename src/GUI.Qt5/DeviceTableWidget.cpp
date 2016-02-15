#include "DeviceTableWidget.h"

DeviceTableWidget::DeviceTableWidget(QWidget * parent)
  : QTableWidget(parent)
{
}

DeviceTableWidget::~DeviceTableWidget()
{
}

void DeviceTableWidget::setVisibleAttributes(const QStringList& names)
{
  _visible_attributes = names;
}

void DeviceTableWidget::insertDevice(const usbguard::Rule& device_rule)
{
}

void DeviceTableWidget::updateDevice(uint32_t id, const usbguard::Rule& device_rule)
{
}

void DeviceTableWidget::deleteDevice(uint32_t id)
{
}

const QList<uint32_t> DeviceTableWidget::getSelectedDeviceIDs() const
{
  QList<uint32_t> device_ids;
  return device_ids;
}

usbguard::Rule::Target DeviceTableWidget::getDeviceTarget(uint32_t id) const
{
  return usbguard::Rule::Target::Block;
}

void DeviceTableWidget::reset()
{
}

void DeviceTableWidget::insertRow(int i, const usbguard::Rule& rule)
{
}

void DeviceTableWidget::updateRow(int i, const usbguard::Rule& rule)
{
}

void DeviceTableWidget::deleteRow(int i)
{
}

