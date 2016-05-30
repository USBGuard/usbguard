//
// Copyright (C) 2016 Red Hat, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Authors: Daniel Kopecek <dkopecek@redhat.com>
//
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

