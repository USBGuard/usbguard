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

#include "DeviceModel.h"
#include "TargetDelegate.h"

#include <QSystemTrayIcon>
#include <QMainWindow>
#include <QTimer>
#include <QSettings>
#include <IPCClient.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public usbguard::IPCClient
{
  using IPCClient::allowDevice;
  using IPCClient::blockDevice;
  using IPCClient::rejectDevice;

  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

signals:
  void uiDeviceInserted(quint32 id, const std::map<std::string, std::string>& attributes, const std::vector<usbguard::USBInterfaceType>& interfaces, bool rule_match);
  void uiDevicePresent(quint32 id, const std::map<std::string, std::string>& attributes, const std::vector<usbguard::USBInterfaceType>& interfaces, usbguard::Rule::Target target);
  void uiDeviceRemoved(quint32 id, const std::map<std::string, std::string>& attributes);
  void uiDeviceAllowed(quint32 id, const std::map<std::string, std::string>& attributes);
  void uiDeviceBlocked(quint32 id, const std::map<std::string, std::string>& attributes);
  void uiDeviceRejected(quint32 id, const std::map<std::string, std::string>& attributes);
  void uiConnected();
  void uiDisconnected();

protected slots:
  void switchVisibilityState(QSystemTrayIcon::ActivationReason reason);
  void flashStep();
  void ipcTryConnect();

  void showDeviceDialog(quint32 id, const std::map<std::string, std::string>& attributes, const std::vector<usbguard::USBInterfaceType>& interfaces, bool rule_match);
  void showMessage(const QString &message, bool alert = false);

  void notifyInserted(quint32 id, const std::map<std::string, std::string>& attributes, const std::vector<usbguard::USBInterfaceType>& interfaces, bool rule_matched);
  void notifyPresent(quint32 id, const std::map<std::string, std::string>& attributes, const std::vector<usbguard::USBInterfaceType>& interfaces, usbguard::Rule::Target target);
  void notifyRemoved(quint32 id, const std::map<std::string, std::string>& attributes);
    
  void notifyAllowed(quint32 id, const std::map<std::string, std::string>& attributes);
  void notifyBlocked(quint32 id, const std::map<std::string, std::string>& attributes);
  void notifyRejected(quint32 id, const std::map<std::string, std::string>& attributes);

  void notifyIPCConnected();
  void notifyIPCDisconnected();

  void allowDevice(quint32 id, bool append);
  void blockDevice(quint32 id, bool append);
  void rejectDevice(quint32 id, bool append);

  void handleIPCConnect();
  void handleIPCDisconnect();

  void handleDeviceInsert(quint32 id);
  void handleDeviceAllow(quint32 id);
  void handleDeviceBlock(quint32 id);
  void handleDeviceRemove(quint32 id);

  void loadSettings();
  void saveSettings();

  void loadDeviceList();
  void editDeviceListRow(const QModelIndex &index);
  void commitDeviceListChanges();
  void clearDeviceList();
  void resetDeviceList();

protected:
  void changeEvent(QEvent *e) override;
  void setupSystemTray();
  void setupSettingsWatcher();
  void startFlashing();
  void stopFlashing();

  void DeviceInserted(quint32 id, const std::map<std::string, std::string>& attributes, const std::vector<usbguard::USBInterfaceType>& interfaces, bool rule_match, quint32 rule_id) override;
  void DevicePresent(quint32 id, const std::map<std::string, std::string>& attributes, const std::vector<usbguard::USBInterfaceType>& interfaces, usbguard::Rule::Target target) override;
  void DeviceRemoved(quint32 id, const std::map<std::string, std::string>& attributes) override;

  void DeviceAllowed(quint32 id, const std::map<std::string, std::string>& attributes, bool rule_match, quint32 rule_id) override;
  void DeviceBlocked(quint32 id, const std::map<std::string, std::string>& attributes, bool rule_match, quint32 rule_id) override;
  void DeviceRejected(quint32 id, const std::map<std::string ,std::string>& attributes, bool rule_match, quint32 rule_id) override;

  void IPCConnected() override;
  void IPCDisconnected(bool exception_initiated, const usbguard::IPCException& exception) override;

private:
  Ui::MainWindow *ui;
  QSystemTrayIcon *systray;
  QTimer _flash_timer;
  bool _flash_state;
  QTimer _ipc_timer;
  QSettings _settings;
  DeviceModel _device_model;
  TargetDelegate _target_delegate;
};

