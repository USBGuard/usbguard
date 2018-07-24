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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "DeviceDialog.h"
#include "DeviceDialog.ui.h"

#include "usbguard/Logger.hpp"

#include <QStyle>
#include <QDesktopWidget>

DeviceDialog::DeviceDialog(quint32 id, QWidget* parent) :
  QDialog(parent),
  ui(new Ui::DeviceDialog)
{
  USBGUARD_LOG(Trace) << "Creating DeviceDialog for device_id=" << id;
  ui->setupUi(this);
  setWindowTitle(QString(tr("USB Device Inserted")));
  setWindowIcon(QIcon(":/usbguard-icon.svg"));
  setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
  connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
  device_id = id;
  setDecisionMethod(DecisionMethod::Buttons);
  setDefaultDecisionTimeout(23);
  setRandomizePosition(false);
  setDefaultDecision(usbguard::Rule::Target::Block);
  updateDialog();
  timer.start(1000);
}

void DeviceDialog::setName(const QString& name)
{
  ui->name_label->setText(name);
}

void DeviceDialog::setDeviceID(const QString& vendor_id, const QString& product_id)
{
  ui->deviceid_label->setText(QString("%1:%2").arg(vendor_id).arg(product_id));
}

void DeviceDialog::setSerial(const QString& serial)
{
  USBGUARD_LOG(Trace) << "Masking serial number value";
  _serial = serial;

  if (_mask_serial_number) {
    for (auto i = _serial.size(), p = 1; i > 0; --i, ++p) {
      if ((p % 2) == 0) {
        _serial[i - 1] = '*';
      }
    }
  }

  ui->serial_label->setText(_serial);
}

void DeviceDialog::setInterfaceTypes(const std::vector<usbguard::USBInterfaceType>& interfaces)
{
  ui->interface_list->clear();

  for (auto const& type : interfaces) {
    ui->interface_list->addItem(QString::fromStdString(type.typeString()));
  }
}

void DeviceDialog::setDefaultDecision(usbguard::Rule::Target target)
{
  switch (target) {
  case usbguard::Rule::Target::Allow:
    ui->allow_button->setFocus();
    break;

  case usbguard::Rule::Target::Block:
    ui->block_button->setFocus();
    break;

  case usbguard::Rule::Target::Reject:
  case usbguard::Rule::Target::Unknown:
  case usbguard::Rule::Target::Empty:
  case usbguard::Rule::Target::Invalid:
  case usbguard::Rule::Target::Match:
  case usbguard::Rule::Target::Device:
  default:
    ui->reject_button->setFocus();
  }

  _default_decision = target;
}

void DeviceDialog::setDefaultDecisionTimeout(quint32 seconds)
{
  _default_decision_timeout = seconds;
  time_left = seconds;
}

void DeviceDialog::setDecisionMethod(DeviceDialog::DecisionMethod method)
{
  _decision_method = method;
}

void DeviceDialog::setDecisionIsPermanent(bool state)
{
  _decision_is_permanent = state;
  ui->permanent_checkbox->setChecked(state);
}

void DeviceDialog::setRejectVisible(bool state)
{
  ui->reject_button->setHidden(!state);
}

static int randomInteger(int a, int b)
{
  return (qrand() % (b > a ? b - a : a - b)) + std::min(a, b);
}

void DeviceDialog::setRandomizePosition(bool randomize)
{
  QRect position_rect = \
    QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
      size(), qApp->desktop()->availableGeometry());

  if (randomize) {
    const int h = ui->block_button->height();
    const int w = ui->block_button->width();
    const int dy = randomInteger(-2.618*h, 2.618*h);
    const int dx = randomInteger(-2.618*w, 2.618*w);
    position_rect.translate(dx, dy);
  }

  setGeometry(position_rect);
}

void DeviceDialog::setMaskSerialNumber(bool state)
{
  _mask_serial_number = state;
}

void DeviceDialog::timerUpdate()
{
  if (time_left > 0) {
    --time_left;
    updateDialog();
  }
  else {
    timer.stop();
    executeDefaultDecision();
  }
}

void DeviceDialog::reject()
{
  if (timer.isActive()) {
    timer.stop();
    updateDialog();
  }
  else {
    QDialog::reject();
  }
}

void DeviceDialog::accept()
{
  if (timer.isActive()) {
    timer.stop();
  }

  QDialog::accept();
}

void DeviceDialog::updateDialog()
{
  QPushButton* button = nullptr;
  QString label;

  switch (_default_decision) {
  case usbguard::Rule::Target::Allow:
    button = ui->allow_button;
    label = QString(tr("Allow"));
    break;

  case usbguard::Rule::Target::Block:
    button = ui->block_button;
    label = QString(tr("Block"));
    break;

  case usbguard::Rule::Target::Reject:
  case usbguard::Rule::Target::Match:
  case usbguard::Rule::Target::Device:
  case usbguard::Rule::Target::Invalid:
  case usbguard::Rule::Target::Empty:
  case usbguard::Rule::Target::Unknown:
  default:
    button = ui->reject_button;
    label = QString(tr("Reject"));
  }

  if (timer.isActive()) {
    button->setText(QString("%1 [%2]").arg(label).arg(time_left));
  }
  else {
    button->setText(label);
    ui->hint_label->setText(tr("(Press Escape to close this window)"));
  }
}

void DeviceDialog::executeDefaultDecision()
{
  USBGUARD_LOG(Debug) << "Executing default decision: "
    << usbguard::Rule::targetToString(_default_decision);

  switch (_default_decision) {
  case usbguard::Rule::Target::Allow:
    on_allow_button_clicked();
    break;

  case usbguard::Rule::Target::Block:
    on_block_button_clicked();
    break;

  case usbguard::Rule::Target::Reject:
  case usbguard::Rule::Target::Unknown:
  case usbguard::Rule::Target::Empty:
  case usbguard::Rule::Target::Invalid:
  case usbguard::Rule::Target::Device:
  case usbguard::Rule::Target::Match:
  default:
    on_allow_button_clicked();
  }
}

DeviceDialog::~DeviceDialog()
{
  delete ui;
}

void DeviceDialog::on_allow_button_clicked()
{
  emit allowed(device_id, ui->permanent_checkbox->isChecked());
  accept();
}

void DeviceDialog::on_block_button_clicked()
{
  emit blocked(device_id, ui->permanent_checkbox->isChecked());
  accept();
}

void DeviceDialog::on_reject_button_clicked()
{
  emit rejected(device_id, ui->permanent_checkbox->isChecked());
  accept();
}

/* vim: set ts=2 sw=2 et */
