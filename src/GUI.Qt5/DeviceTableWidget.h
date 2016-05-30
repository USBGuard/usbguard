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

