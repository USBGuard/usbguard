//
// Copyright (C) 2015 Red Hat, Inc.
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

#include <QDialog>
#include <QTimer>
#include <USB.hpp>

namespace Ui {
class DeviceDialog;
}

class DeviceDialog : public QDialog
{
  Q_OBJECT

public:
  explicit DeviceDialog(quint32 seqn, QWidget *parent = 0);
  ~DeviceDialog();

  void setName(const QString& name);
  void setSerial(const QString& serial);
  void setDeviceID(const QString& vendor_id, const QString& product_id);
  void setInterfaceTypes(const std::vector<usbguard::USBInterfaceType>& interfaces);

signals:
  void allowed(quint32 seqn, bool permanent);
  void blocked(quint32 seqn, bool permanent);
  void rejected(quint32 seqn, bool permanent);

protected slots:
  void timerUpdate();

protected:
  void reject();
  void accept();
  void updateDialog();
  void executeDefaultDecision();

private slots:
  void on_allow_button_clicked();
  void on_block_button_clicked();
  void on_reject_button_clicked();
  void on_timeout_checkbox_toggled(bool checked);
  void on_permanent_checkbox_toggled(bool checked);

private:
  Ui::DeviceDialog *ui;
  QTimer timer;
  int time_left;

  quint32 device_seqn;

  QString _name;
  QString _serial;
  QString _device_id;
  QStringList _interface_types;
};
