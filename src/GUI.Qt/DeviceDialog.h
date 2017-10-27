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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard/USB.hpp"
#include "usbguard/Rule.hpp"

#include <QDialog>
#include <QTimer>

namespace Ui
{
  class DeviceDialog;
}

class DeviceDialog : public QDialog
{
  Q_OBJECT

public:
  enum class DecisionMethod {
    Buttons,
    MathTest
  };

  explicit DeviceDialog(quint32 id, QWidget* parent = 0);
  ~DeviceDialog();

  void setName(const QString& name);
  void setSerial(const QString& serial);
  void setDeviceID(const QString& vendor_id, const QString& product_id);
  void setInterfaceTypes(const std::vector<usbguard::USBInterfaceType>& interfaces);

  void setDefaultDecision(usbguard::Rule::Target target);
  void setDefaultDecisionTimeout(quint32 seconds);
  void setDecisionMethod(DecisionMethod method);
  void setDecisionIsPermanent(bool state);
  void setRejectVisible(bool state);
  void setRandomizePosition(bool randomize);
  void setMaskSerialNumber(bool state);

signals:
  void allowed(quint32 id, bool permanent);
  void blocked(quint32 id, bool permanent);
  void rejected(quint32 id, bool permanent);

protected slots:
  void timerUpdate();

protected:
  void reject();
  void accept();
  void updateDialog();
  void executeDefaultDecision();
  void setPosition(bool randomized);

private slots:
  void on_allow_button_clicked();
  void on_block_button_clicked();
  void on_reject_button_clicked();

private:
  Ui::DeviceDialog* ui;
  usbguard::Rule::Target _default_decision;
  quint32 _default_decision_timeout;
  DecisionMethod _decision_method;
  bool _reject_enabled;
  bool _mask_serial_number;
  bool _decision_is_permanent;

  QTimer timer;
  int time_left;

  quint32 device_id;

  QString _name;
  QString _serial;
  QString _device_id;
  QStringList _interface_types;
};

/* vim: set ts=2 sw=2 et */
