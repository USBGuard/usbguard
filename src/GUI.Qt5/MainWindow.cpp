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
#include "MainWindow.h"
#include "MainWindow.ui.h"
#include "DeviceDialog.h"
#include <QString>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QMessageBox>
#include <QApplication>
#include <QResource>
#include <QMenu>
#include <QAction>
#include <QDateTime>
#include <QTime>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QTreeView>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _settings("USBGuard", "usbguard-applet-qt"),
    _device_model(this)
{
  /*
   * Seed the pseudo-random generator. We use it for
   * randomizing the position of the DeviceDialog window.
   */
  QTime time_rnd_seed = QTime::currentTime();
  qsrand((uint)time_rnd_seed.msec());

  ui->setupUi(this);
  ui->device_view->setModel(&_device_model);
  ui->device_view->setItemDelegateForColumn(2, &_target_delegate);
  ui->device_view->resizeColumnToContents(1);

  QObject::connect(ui->device_view->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                   this, SLOT(editDeviceListRow(QModelIndex)));
  QObject::connect(ui->device_view, SIGNAL(clicked(QModelIndex)),
                   this, SLOT(editDeviceListRow(QModelIndex)));
  QObject::connect(ui->apply_button, SIGNAL(pressed()),
                   this, SLOT(commitDeviceListChanges()));
  QObject::connect(ui->reset_button, SIGNAL(pressed()),
                   this, SLOT(resetDeviceList()));

  setWindowTitle("USBGuard");
  setWindowIcon(QIcon(":/usbguard-icon.svg"));
  setWindowState(Qt::WindowMinimized);
  setupSystemTray();

  qRegisterMetaType<std::map<std::string, std::string> >("std::map<std::string, std::string>");
  qRegisterMetaType<std::vector<usbguard::USBInterfaceType> >("std::vector<usbguard::USBInterfaceType>");

  QObject::connect(&_ipc_timer, SIGNAL(timeout()),
                   this, SLOT(ipcTryConnect()));

  QObject::connect(this, SIGNAL(uiDeviceInserted(quint32, const std::map<std::string, std::string>&, const std::vector<usbguard::USBInterfaceType>&, bool)),
                   this, SLOT(showDeviceDialog(quint32, const std::map<std::string, std::string>&, const std::vector<usbguard::USBInterfaceType>&, bool)));
  QObject::connect(this, SIGNAL(uiDeviceInserted(quint32, const std::map<std::string, std::string>&, const std::vector<usbguard::USBInterfaceType>&, bool)),
                   this, SLOT(handleDeviceInsert(quint32)));

  QObject::connect(this, SIGNAL(uiDevicePresent(quint32, const std::map<std::string, std::string>&, const std::vector<usbguard::USBInterfaceType>&, usbguard::Rule::Target)),
                   this, SLOT(notifyPresent(quint32, const std::map<std::string, std::string>&, const std::vector<usbguard::USBInterfaceType>&, usbguard::Rule::Target)));

  QObject::connect(this, SIGNAL(uiDeviceInserted(quint32, const std::map<std::string, std::string>&, const std::vector<usbguard::USBInterfaceType>&, bool)),
                   this, SLOT(notifyInserted(quint32, const std::map<std::string, std::string>&, const std::vector<usbguard::USBInterfaceType>&, bool)));

  QObject::connect(this, SIGNAL(uiDeviceAllowed(quint32, const std::map<std::string, std::string>&)),
                   this, SLOT(notifyAllowed(quint32, const std::map<std::string, std::string>&)));
  QObject::connect(this, SIGNAL(uiDeviceAllowed(quint32, const std::map<std::string, std::string>&)),
                   this, SLOT(handleDeviceAllow(quint32)));

  QObject::connect(this, SIGNAL(uiDeviceBlocked(quint32, const std::map<std::string, std::string>&)),
                   this, SLOT(notifyBlocked(quint32, const std::map<std::string, std::string>&)));
  QObject::connect(this, SIGNAL(uiDeviceBlocked(quint32, const std::map<std::string, std::string>&)),
                   this, SLOT(handleDeviceBlock(quint32)));

  QObject::connect(this, SIGNAL(uiDeviceRejected(quint32, const std::map<std::string, std::string>&)),
                   this, SLOT(notifyRejected(quint32, const std::map<std::string, std::string>&)));

  QObject::connect(this, SIGNAL(uiDeviceRemoved(quint32, const std::map<std::string, std::string>&)),
                   this, SLOT(notifyRemoved(quint32, const std::map<std::string, std::string>&)));
  QObject::connect(this, SIGNAL(uiDeviceRemoved(quint32, const std::map<std::string, std::string>&)),
                   this, SLOT(handleDeviceRemove(quint32)));

  QObject::connect(this, SIGNAL(uiConnected()),
                   this, SLOT(handleIPCConnect()));

  QObject::connect(this, SIGNAL(uiDisconnected()),
                   this, SLOT(handleIPCDisconnect()));

  setupSettingsWatcher();

  loadSettings();

  _ipc_timer.setInterval(1000);
  _ipc_timer.start();
  ui->statusBar->showMessage(tr("Inactive. No IPC connection."));

  new QShortcut(QKeySequence(Qt::Key_Escape, Qt::Key_Escape), this, SLOT(showMinimized()));
  return;
}

void MainWindow::setupSystemTray()
{
  systray = new QSystemTrayIcon(QIcon(":/usbguard-icon-inactive.svg"), this);
  systray->setToolTip("USBGuard");
  auto menu = new QMenu();
  auto quit_action = new QAction(tr("Quit"), systray);
  menu->addAction(quit_action);
  systray->setContextMenu(menu);

  QObject::connect(quit_action, SIGNAL(triggered()), qApp, SLOT(quit()));
  QObject::connect(systray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                   this, SLOT(switchVisibilityState(QSystemTrayIcon::ActivationReason)));
  QObject::connect(&_flash_timer, SIGNAL(timeout()),
                   this, SLOT(flashStep()));    
  systray->show();
  return;
}

void MainWindow::setupSettingsWatcher()
{
  for (QCheckBox* checkbox : ui->settings_tab->findChildren<QCheckBox*>()) {
      QObject::connect(checkbox, SIGNAL(toggled(bool)), this, SLOT(saveSettings()));
  }
  for (QComboBox* combobox : ui->settings_tab->findChildren<QComboBox*>()) {
      QObject::connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(saveSettings()));
  }
  for (QSpinBox* spinbox : ui->settings_tab->findChildren<QSpinBox*>()) {
      QObject::connect(spinbox, SIGNAL(valueChanged(int)), this, SLOT(saveSettings()));
  }
}

void MainWindow::switchVisibilityState(QSystemTrayIcon::ActivationReason reason)
{
  if (reason == QSystemTrayIcon::Context) {
    systray->contextMenu()->show();
    return;
  } else {
    if (windowState() & Qt::WindowMinimized) {
      showNormal();
      stopFlashing();
    } else {
      showMinimized();
    }
  }
}

MainWindow::~MainWindow()
{
  IPCClient::disconnect();
  delete ui;
}

void MainWindow::showDeviceDialog(quint32 id, const std::map<std::string, std::string>& attributes, const std::vector<usbguard::USBInterfaceType>& interfaces, bool rule_match)
{
  if (rule_match) {
    return;
  }

  auto dialog = new DeviceDialog(id);

  dialog->setRejectVisible(ui->show_reject_button_checkbox->isChecked());
  dialog->setDefaultDecisionTimeout(ui->decision_timeout->value());
  dialog->setMaskSerialNumber(ui->mask_serial_checkbox->isChecked());
  dialog->setDecisionIsPermanent(ui->decision_permanent_checkbox->isChecked());

  dialog->setName(QString::fromStdString(attributes.at("name")));
  dialog->setSerial(QString::fromStdString(attributes.at("serial")));
  dialog->setDeviceID(QString::fromStdString(attributes.at("vendor_id")),
                 QString::fromStdString(attributes.at("product_id")));
  dialog->setInterfaceTypes(interfaces);

  dialog->setModal(false);
  dialog->setRandomizePosition(ui->randomize_position_checkbox->isChecked());
 
  QObject::connect(dialog, SIGNAL(allowed(quint32,bool)),
                   this, SLOT(allowDevice(quint32,bool)));
  QObject::connect(dialog, SIGNAL(rejected(quint32,bool)),
                   this, SLOT(rejectDevice(quint32,bool)));
  QObject::connect(dialog, SIGNAL(blocked(quint32,bool)),
                   this, SLOT(blockDevice(quint32,bool)));

  dialog->show();
  dialog->raise();
  dialog->activateWindow();

  return;
}

void MainWindow::showMessage(const QString& message, bool alert)
{
  const QString text("[%1] %2%3%4");
  const QString dt_string = QDateTime::currentDateTime().toString();
  ui->messages_text->append(text.arg(dt_string).arg(alert?"<b>":"").arg(message).arg(alert?"</b>":""));
  return;
}

void MainWindow::notifyInserted(quint32 id, const std::map<std::string, std::string>& attributes, const std::vector<usbguard::USBInterfaceType>& interfaces, bool rule_matched)
{
  if (ui->notify_inserted->isChecked()) {
    if (rule_matched) {
      systray->showMessage(tr("USB Device Inserted"), QString(tr("Name: %1")).arg(QString::fromStdString(attributes.at("name"))), QSystemTrayIcon::Information);
    }
  }
  showMessage(QString(tr("<i>Inserted</i>: %1")).arg(QString::fromStdString(attributes.at("name"))));
  return;
}

void MainWindow::notifyPresent(quint32 id, const std::map<std::string, std::string>& attributes, const std::vector<usbguard::USBInterfaceType>& interfaces, usbguard::Rule::Target target)
{
  if (ui->notify_present->isChecked()) {
    systray->showMessage(tr("USB Device Present"), QString(tr("Name: %1")).arg(QString::fromStdString(attributes.at("name"))), QSystemTrayIcon::Information);
  }
  showMessage(QString(tr("<i>Present</i>: %1")).arg(QString::fromStdString(attributes.at("name"))));
  return;
}

void MainWindow::notifyRemoved(quint32 id, const std::map<std::string, std::string>& attributes)
{
  if (ui->notify_removed->isChecked()) {
    systray->showMessage(tr("USB Device Removed"), QString(tr("Name: %1")).arg(QString::fromStdString(attributes.at("name"))), QSystemTrayIcon::Information);
  }
  showMessage(QString(tr("<i>Removed</i>: %1")).arg(QString::fromStdString(attributes.at("name"))));
  return;
}

void MainWindow::notifyAllowed(quint32 id, const std::map<std::string, std::string>& attributes)
{
  if (ui->notify_allowed->isChecked()) {
    systray->showMessage(tr("USB Device Allowed"), QString(tr("Name: %1")).arg(QString::fromStdString(attributes.at("name"))), QSystemTrayIcon::Information);
  }
  showMessage(QString(tr("Allowed: %1")).arg(QString::fromStdString(attributes.at("name"))));
  return;
}

void MainWindow::notifyBlocked(quint32 id, const std::map<std::string, std::string>& attributes)
{
  if (ui->notify_blocked->isChecked()) {
    systray->showMessage(tr("USB Device Blocked"), QString(tr("Name: %1")).arg(QString::fromStdString(attributes.at("name"))), QSystemTrayIcon::Warning);
  }
  showMessage(QString(tr("Blocked: %1")).arg(QString::fromStdString(attributes.at("name"))));
  return;
}

void MainWindow::notifyRejected(quint32 id, const std::map<std::string, std::string>& attributes)
{
  if (ui->notify_rejected->isChecked()) {
    systray->showMessage(tr("USB Device Rejected"), QString(tr("Name: %1")).arg(QString::fromStdString(attributes.at("name"))), QSystemTrayIcon::Critical);
  }
  showMessage(QString(tr("Rejected: %1")).arg(QString::fromStdString(attributes.at("name"))), true);
  if (windowState() & Qt::WindowMinimized) {
    startFlashing();
  }
  return;
}

void MainWindow::notifyIPCConnected()
{
  if (ui->notify_ipc->isChecked()) {
    systray->showMessage(tr("IPC Connection Established"), "", QSystemTrayIcon::Information);
  }
  showMessage(tr("IPC connection established"));
  ui->statusBar->showMessage(tr("IPC connection established."));
}

void MainWindow::notifyIPCDisconnected()
{
  if (ui->notify_ipc->isChecked()) {
    systray->showMessage(tr("IPC Connection Lost"), "", QSystemTrayIcon::Information);
  }
  showMessage(tr("IPC connection lost"));
  ui->statusBar->showMessage(tr("Inactive. No IPC connection."));
}

void MainWindow::startFlashing()
{
  _flash_state = false;
  _flash_timer.setInterval(500);
  _flash_timer.start();
}

void MainWindow::stopFlashing()
{
  _flash_state = false;
  _flash_timer.stop();
  if (IPCClient::isConnected()) {
    systray->setIcon(QIcon(":/usbguard-icon.svg"));
  }
  else {
    systray->setIcon(QIcon(":/usbguard-icon-inactive.svg"));
  }
}

void MainWindow::flashStep()
{
  if (_flash_state) {
    systray->setIcon(QIcon(":/usbguard-icon-warning.svg"));
    systray->show();
    _flash_timer.setInterval(250);
    _flash_state = false;
  }
  else {
    if (IPCClient::isConnected()) {
      systray->setIcon(QIcon(":/usbguard-icon.svg"));
    }
    else {
      systray->setIcon(QIcon(":/usbguard-icon-inactive.svg"));
    }
    systray->show();
    _flash_timer.setInterval(500);
    _flash_state = true;
  }
}

void MainWindow::ipcTryConnect()
{
  try {
    IPCClient::connect();
  }
  catch(const usbguard::IPCException& ex) {
    showMessage(QString("IPC connection failed: %1: %2")
                .arg(QString::fromStdString(ex.codeAsString()))
                .arg(QString::fromStdString(ex.message())),
                /*alert=*/true);
  }
  catch(const std::exception& ex) {
    showMessage(QString("IPC connection failed: std::exception: %1")
                .arg(QString::fromStdString(ex.what())),
                /*alert=*/true);
  }
}

void MainWindow::allowDevice(quint32 id, bool permanent)
{
  try {
    IPCClient::allowDevice(id, permanent, 0);
  }
  catch(const usbguard::IPCException& ex) {
    showMessage(QString("IPC call failed: %1: %2: %3")
                .arg("allowDevice")
                .arg(QString::fromStdString(ex.codeAsString()))
                .arg(QString::fromStdString(ex.message())),
                /*alert=*/true);
  }
  catch(const std::exception& ex) {
    showMessage(QString("IPC call failed: %1: std::exception: %2")
                .arg("allowDevice")
                .arg(QString::fromStdString(ex.what())),
                /*alert=*/true);
  }
}

void MainWindow::blockDevice(quint32 id, bool permanent)
{
  try {
    IPCClient::blockDevice(id, permanent, 0);
  }
  catch(const usbguard::IPCException& ex) {
    showMessage(QString("IPC call failed: %1: %2: %3")
                .arg("blockDevice")
                .arg(QString::fromStdString(ex.codeAsString()))
                .arg(QString::fromStdString(ex.message())),
                /*alert=*/true);
  }
  catch(const std::exception& ex) {
    showMessage(QString("IPC call failed: %1: std::exception: %2")
                .arg("blockDevice")
                .arg(QString::fromStdString(ex.what())),
                /*alert=*/true);
  }
}

void MainWindow::rejectDevice(quint32 id, bool permanent)
{
  try {
    IPCClient::rejectDevice(id, permanent, 0);
  }
  catch(const usbguard::IPCException& ex) {
    showMessage(QString("IPC call failed: %1: %2: %3")
                .arg("rejectDevice")
                .arg(QString::fromStdString(ex.codeAsString()))
                .arg(QString::fromStdString(ex.message())),
                /*alert=*/true);
  }
  catch(const std::exception& ex) {
    showMessage(QString("IPC call failed: %1: std::exception: %2")
                .arg("rejectDevice")
                .arg(QString::fromStdString(ex.what())),
                /*alert=*/true);
  }
}

void MainWindow::handleIPCConnect()
{
  _ipc_timer.stop();
  notifyIPCConnected();
  systray->setIcon(QIcon(":/usbguard-icon.svg"));
  ui->device_view->setDisabled(false);
  loadDeviceList();
}

void MainWindow::handleIPCDisconnect()
{
  _ipc_timer.start();
  notifyIPCDisconnected();
  systray->setIcon(QIcon(":/usbguard-icon-inactive.svg"));
  clearDeviceList();
  ui->device_view->setDisabled(true);
}

void MainWindow::handleDeviceInsert(quint32 id)
{
  loadDeviceList();
}

void MainWindow::handleDeviceAllow(quint32 id)
{
  _device_model.updateDeviceTarget(id, usbguard::Rule::Target::Allow);
}

void MainWindow::handleDeviceBlock(quint32 id)
{
  _device_model.updateDeviceTarget(id, usbguard::Rule::Target::Block);
}

void MainWindow::handleDeviceRemove(quint32 id)
{
  ui->device_view->selectionModel()->clearSelection();
  ui->device_view->reset();
  _device_model.removeDevice(id);
}

void MainWindow::loadSettings()
{
  _settings.beginGroup("Notifications");
  ui->notify_inserted->setChecked(_settings.value("Inserted", true).toBool());
  ui->notify_removed->setChecked(_settings.value("Removed", false).toBool());
  ui->notify_allowed->setChecked(_settings.value("Allowed", true).toBool());
  ui->notify_blocked->setChecked(_settings.value("Blocked", true).toBool());
  ui->notify_rejected->setChecked(_settings.value("Rejected", true).toBool());
  ui->notify_present->setChecked(_settings.value("Present", false).toBool());
  ui->notify_ipc->setChecked(_settings.value("IPCStatus", false).toBool());
  _settings.endGroup();

  _settings.beginGroup("DeviceDialog");
  ui->default_decision_combobox->setCurrentText(_settings.value("DefaultDecision", QString("block")).toString());
  ui->decision_timeout->setValue(_settings.value("DefaultDecisionTimeout", 23).toInt());
  ui->decision_method_combobox->setCurrentText(_settings.value("DecisionMethod", QString("Buttons")).toString());
  ui->decision_permanent_checkbox->setChecked(_settings.value("DecisionIsPermanent", false).toBool());
  ui->show_reject_button_checkbox->setChecked(_settings.value("ShowRejectButton", false).toBool());
  ui->randomize_position_checkbox->setChecked(_settings.value("RandomizeWindowPosition", true).toBool());
  ui->mask_serial_checkbox->setChecked(_settings.value("MaskSerialNumber", true).toBool());
  _settings.endGroup();
}

void MainWindow::saveSettings()
{
  _settings.beginGroup("Notifications");
  _settings.setValue("Inserted", ui->notify_inserted->isChecked());
  _settings.setValue("Removed", ui->notify_removed->isChecked());
  _settings.setValue("Allowed", ui->notify_allowed->isChecked());
  _settings.setValue("Blocked", ui->notify_blocked->isChecked());
  _settings.setValue("Rejected", ui->notify_rejected->isChecked());
  _settings.setValue("Present", ui->notify_present->isChecked());
  _settings.setValue("IPCStatus", ui->notify_ipc->isChecked());
  _settings.endGroup();

  _settings.beginGroup("DeviceDialog");
  _settings.setValue("DefaultDecision", ui->default_decision_combobox->currentText());
  _settings.setValue("DefaultDecisionTimeout", ui->decision_timeout->value());
  _settings.setValue("DecisionMethod", ui->decision_method_combobox->currentText());
  _settings.setValue("DecisionIsPermanent", ui->decision_permanent_checkbox->isChecked());
  _settings.setValue("ShowRejectButton", ui->show_reject_button_checkbox->isChecked());
  _settings.setValue("RandomizeWindowPosition", ui->randomize_position_checkbox->isChecked());
  _settings.setValue("MaskSerialNumber", ui->mask_serial_checkbox->isChecked());
  _settings.endGroup();
}

void MainWindow::loadDeviceList()
{
  try {
    for (auto device_rule : IPCClient::listDevices()) {
      if (!_device_model.containsDevice(device_rule.getRuleID())) {
        _device_model.insertDevice(device_rule);
      }
    }
    ui->device_view->expandToDepth(2);
  }
  catch(const usbguard::IPCException& ex) {
    showMessage(QString("IPC call failed: %1: %2: %3")
                .arg("listDevices")
                .arg(QString::fromStdString(ex.codeAsString()))
                .arg(QString::fromStdString(ex.message())),
                /*alert=*/true);
  }
  catch(const std::exception& ex) {
    showMessage(QString("IPC call failed: %1: std::exception: %2")
                .arg("listDevices")
                .arg(QString::fromStdString(ex.what())),
                /*alert=*/true);
  }
}

void MainWindow::editDeviceListRow(const QModelIndex &index)
{
  ui->device_view->edit(_device_model.createRowEditIndex(index));
}

void MainWindow::commitDeviceListChanges()
{
  auto modified_map = _device_model.getModifiedDevices();
  auto modified_it = modified_map.begin();

  while (modified_it != modified_map.end()) {
    auto id = modified_it.key();
    auto target = modified_it.value();

    switch(target)
    {
      case usbguard::Rule::Target::Allow:
        allowDevice(id, /*permanent=*/false);
        break;
      case usbguard::Rule::Target::Block:
        blockDevice(id, /*permanent=*/false);
        break;
      case usbguard::Rule::Target::Reject:
        rejectDevice(id, /*permanent=*/false);
        break;
      default:
        break;
    }

    ++modified_it;
  }
}

void MainWindow::clearDeviceList()
{
  ui->device_view->clearSelection();
  ui->device_view->reset();
  _device_model.clear();
}

void MainWindow::resetDeviceList()
{
  clearDeviceList();
  loadDeviceList();
}

void MainWindow::changeEvent(QEvent* e)
{
  switch (e->type()) {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;
    case QEvent::WindowStateChange:
      if (windowState() & Qt::WindowMinimized) {
        QTimer::singleShot(250, this, SLOT(hide()));
      }
      break;
    default:
      break;
  }
  QMainWindow::changeEvent(e);
  return;
}

void MainWindow::DeviceInserted(quint32 id, const std::map<std::string, std::string>& attributes, const std::vector<usbguard::USBInterfaceType>& interfaces, bool rule_match, quint32 rule_id)
{
  emit uiDeviceInserted(id, attributes, interfaces, rule_match);
}

void MainWindow::DevicePresent(quint32 id, const std::map<std::string, std::string>& attributes, const std::vector<usbguard::USBInterfaceType>& interfaces, usbguard::Rule::Target target)
{
  emit uiDevicePresent(id, attributes, interfaces, target);
}

void MainWindow::DeviceRemoved(quint32 id, const std::map<std::string, std::string>& attributes)
{
  emit uiDeviceRemoved(id, attributes);
}

void MainWindow::DeviceAllowed(quint32 id, const std::map<std::string, std::string>& attributes, bool rule_match, quint32 rule_id)
{
  emit uiDeviceAllowed(id, attributes);
}

void MainWindow::DeviceBlocked(quint32 id, const std::map<std::string, std::string>& attributes, bool rule_match, quint32 rule_id)
{
  emit uiDeviceBlocked(id, attributes);
}

void MainWindow::DeviceRejected(quint32 id, const std::map<std::string, std::string>& attributes, bool rule_match, quint32 rule_id)
{
  emit uiDeviceRejected(id, attributes);
}

void MainWindow::IPCConnected()
{
  emit uiConnected();
}

void MainWindow::IPCDisconnected(bool exception_initiated, const usbguard::IPCException& exception)
{
  emit uiDisconnected();
}
