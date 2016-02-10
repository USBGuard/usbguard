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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("USBGuard");
    setWindowIcon(QIcon(":/usbguard-icon.svg"));
    setupSystemTray();
    showMinimized();

    qRegisterMetaType<std::map<std::string, std::string> >("std::map<std::string, std::string>");
    qRegisterMetaType<std::vector<usbguard::USBInterfaceType> >("std::vector<usbguard::USBInterfaceType>");

    QObject::connect(&_ipc_timer, SIGNAL(timeout()),
		     this, SLOT(ipcTryConnect()));

    QObject::connect(this, SIGNAL(uiDeviceInserted(quint32, const std::map<std::string, std::string>&, const std::vector<usbguard::USBInterfaceType>&, bool)),
		     this, SLOT(showDeviceDialog(quint32, const std::map<std::string, std::string>&, const std::vector<usbguard::USBInterfaceType>&, bool)));

    QObject::connect(this, SIGNAL(uiDevicePresent(quint32, const std::map<std::string, std::string>&, const std::vector<usbguard::USBInterfaceType>&, usbguard::Rule::Target)),
		     this, SLOT(notifyPresent(quint32, const std::map<std::string, std::string>&, const std::vector<usbguard::USBInterfaceType>&, usbguard::Rule::Target)));

    QObject::connect(this, SIGNAL(uiDeviceInserted(quint32, const std::map<std::string, std::string>&, const std::vector<usbguard::USBInterfaceType>&, bool)),
		     this, SLOT(notifyInserted(quint32, const std::map<std::string, std::string>&, const std::vector<usbguard::USBInterfaceType>&, bool)));

    QObject::connect(this, SIGNAL(uiDeviceAllowed(quint32, const std::map<std::string, std::string>&)),
		     this, SLOT(notifyAllowed(quint32, const std::map<std::string, std::string>&)));

    QObject::connect(this, SIGNAL(uiDeviceBlocked(quint32, const std::map<std::string, std::string>&)),
		     this, SLOT(notifyBlocked(quint32, const std::map<std::string, std::string>&)));

    QObject::connect(this, SIGNAL(uiDeviceRejected(quint32, const std::map<std::string, std::string>&)),
		     this, SLOT(notifyRejected(quint32, const std::map<std::string, std::string>&)));

    QObject::connect(this, SIGNAL(uiDeviceRemoved(quint32, const std::map<std::string, std::string>&)),
		     this, SLOT(notifyRemoved(quint32, const std::map<std::string, std::string>&)));

    QObject::connect(this, SIGNAL(uiConnected()),
		     this, SLOT(handleIPCConnect()));

    QObject::connect(this, SIGNAL(uiDisconnected()),
		     this, SLOT(handleIPCDisconnect()));

    _ipc_timer.setInterval(1000);
    _ipc_timer.start();
    ui->statusBar->showMessage(tr("Inactive. No IPC connection."));

#if 1
    /* Hide things which arent' working yet */
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(1);
    ui->device_tab->hide();
    ui->settings_tab->hide();
#endif
    return;
}

void MainWindow::setupSystemTray()
{
    systray = new QSystemTrayIcon(QIcon(":/usbguard-icon.svg"), this);
    systray->setToolTip("USBGuard");
    auto menu = new QMenu();
    auto quit_action = new QAction(tr("Quit"), systray);
    menu->addAction(quit_action);
    systray->setContextMenu(menu);

    QObject::connect(quit_action, SIGNAL(triggered()),
		     qApp, SLOT(quit()));

    QObject::connect(systray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                     this, SLOT(switchVisibilityState(QSystemTrayIcon::ActivationReason)));

    QObject::connect(&_flash_timer, SIGNAL(timeout()),
		     this, SLOT(flashStep()));    

    systray->show();
    return;
}

void MainWindow::switchVisibilityState(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Context) {
        systray->contextMenu()->show();
        return;
    } else {
        if (this->windowState() & Qt::WindowMinimized) {
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

void MainWindow::showDeviceDialog(quint32 seqn, const std::map<std::string, std::string>& attributes, const std::vector<usbguard::USBInterfaceType>& interfaces, bool rule_match)
{
    if (!rule_match) {
        DeviceDialog *d = new DeviceDialog(seqn);

        d->setName(QString::fromStdString(attributes.at("name")));
    d->setSerial(QString::fromStdString(attributes.at("serial")));
    d->setDeviceID(QString::fromStdString(attributes.at("vendor_id")),
                   QString::fromStdString(attributes.at("product_id")));
	d->setInterfaceTypes(interfaces);

        QObject::connect(d, SIGNAL(allowed(quint32,bool)),
			 this, SLOT(allowDevice(quint32,bool)));
        QObject::connect(d, SIGNAL(rejected(quint32,bool)),
			 this, SLOT(rejectDevice(quint32,bool)));
        QObject::connect(d, SIGNAL(blocked(quint32,bool)),
			 this, SLOT(blockDevice(quint32,bool)));

        d->setModal(false);
        d->show();
    }
    return;
}

void MainWindow::showMessage(const QString& message, bool alert)
{
    const QString text("[%1] %2%3%4");
    const QString dt_string = QDateTime::currentDateTime().toString();
    ui->messages_text->append(text.arg(dt_string).arg(alert?"<b>":"").arg(message).arg(alert?"</b>":""));

    return;
}

void MainWindow::notifyInserted(quint32 seqn, const std::map<std::string, std::string>& attributes, const std::vector<usbguard::USBInterfaceType>& interfaces, bool rule_matched)
{
  if (rule_matched) {
    systray->showMessage(tr("USB Device Inserted"), QString(tr("Name: %1")).arg(QString::fromStdString(attributes.at("name"))), QSystemTrayIcon::Information);
  }
  showMessage(QString(tr("<i>Inserted</i>: %1")).arg(QString::fromStdString(attributes.at("name"))));
  return;
}

void MainWindow::notifyPresent(quint32 seqn, const std::map<std::string, std::string>& attributes, const std::vector<usbguard::USBInterfaceType>& interfaces, usbguard::Rule::Target target)
{
  systray->showMessage(tr("USB Device Present"), QString(tr("Name: %1")).arg(QString::fromStdString(attributes.at("name"))), QSystemTrayIcon::Information);
  showMessage(QString(tr("<i>Present</i>: %1")).arg(QString::fromStdString(attributes.at("name"))));
  return;
}

void MainWindow::notifyRemoved(quint32 seqn, const std::map<std::string, std::string>& attributes)
{
  systray->showMessage(tr("USB Device Removed"), QString(tr("Name: %1")).arg(QString::fromStdString(attributes.at("name"))), QSystemTrayIcon::Information);
  showMessage(QString(tr("<i>Removed</i>: %1")).arg(QString::fromStdString(attributes.at("name"))));
  return;
}

void MainWindow::notifyAllowed(quint32 seqn, const std::map<std::string, std::string>& attributes)
{
  systray->showMessage(tr("USB Device Allowed"), QString(tr("Name: %1")).arg(QString::fromStdString(attributes.at("name"))), QSystemTrayIcon::Information);
  showMessage(QString(tr("Allowed: %1")).arg(QString::fromStdString(attributes.at("name"))));
  return;
}

void MainWindow::notifyBlocked(quint32 seqn, const std::map<std::string, std::string>& attributes)
{
  systray->showMessage(tr("USB Device Blocked"), QString(tr("Name: %1")).arg(QString::fromStdString(attributes.at("name"))), QSystemTrayIcon::Warning);
  showMessage(QString(tr("Blocked: %1")).arg(QString::fromStdString(attributes.at("name"))));
  return;
}

void MainWindow::notifyRejected(quint32 seqn, const std::map<std::string, std::string>& attributes)
{
  systray->showMessage(tr("USB Device Rejected"), QString(tr("Name: %1")).arg(QString::fromStdString(attributes.at("name"))), QSystemTrayIcon::Critical);
  showMessage(QString(tr("Rejected: %1")).arg(QString::fromStdString(attributes.at("name"))), true);
  if (this->windowState() & Qt::WindowMinimized) {
    startFlashing();
  }
  return;
}

void MainWindow::notifyIPCConnected()
{
  systray->showMessage(tr("IPC Connection Established"), "", QSystemTrayIcon::Information);
  showMessage(tr("IPC connection established"));
  ui->statusBar->showMessage(tr("IPC connection established."));
}

void MainWindow::notifyIPCDisconnected()
{
  systray->showMessage(tr("IPC Connection Lost"), "", QSystemTrayIcon::Information);
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
    systray->setIcon(QIcon(":/usbguard-icon.svg"));
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
        systray->setIcon(QIcon(":/usbguard-icon.svg"));
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
    catch(...) {
    }
}

void MainWindow::allowDevice(quint32 seqn, bool append)
{
    try {
        IPCClient::allowDevice(seqn, append, 0);
    }
    catch(...) {
    }
}

void MainWindow::blockDevice(quint32 seqn, bool append)
{
    try {
        IPCClient::blockDevice(seqn, append, 0);
    }
    catch(...) {
    }
}

void MainWindow::rejectDevice(quint32 seqn, bool append)
{
    try {
        IPCClient::rejectDevice(seqn, append, 0);
    } catch(...) {
    }
}

void MainWindow::handleIPCConnect()
{
    _ipc_timer.stop();
    notifyIPCConnected();
}

void MainWindow::handleIPCDisconnect()
{
    _ipc_timer.start();
    notifyIPCDisconnected();
}

void MainWindow::changeEvent(QEvent* e)
{
    switch (e->type())
    {
        case QEvent::LanguageChange:
            this->ui->retranslateUi(this);
            break;
        case QEvent::WindowStateChange:
            {
                if (this->windowState() & Qt::WindowMinimized)
                {
                    QTimer::singleShot(250, this, SLOT(hide()));
                }
                break;
            }
        default:
            break;
    }

    QMainWindow::changeEvent(e);
}

void MainWindow::DeviceInserted(quint32 seqn, const std::map<std::string, std::string>& attributes, const std::vector<usbguard::USBInterfaceType>& interfaces, bool rule_match, quint32 rule_seqn)
{
  emit uiDeviceInserted(seqn, attributes, interfaces, rule_match);
}

void MainWindow::DevicePresent(quint32 seqn, const std::map<std::string, std::string>& attributes, const std::vector<usbguard::USBInterfaceType>& interfaces, usbguard::Rule::Target target)
{
  emit uiDevicePresent(seqn, attributes, interfaces, target);
}

void MainWindow::DeviceRemoved(quint32 seqn, const std::map<std::string, std::string>& attributes)
{
  emit uiDeviceRemoved(seqn, attributes);
}

void MainWindow::DeviceAllowed(quint32 seqn, const std::map<std::string, std::string>& attributes, bool rule_match, quint32 rule_seqn)
{
  emit uiDeviceAllowed(seqn, attributes);
}

void MainWindow::DeviceBlocked(quint32 seqn, const std::map<std::string, std::string>& attributes, bool rule_match, quint32 rule_seqn)
{
  emit uiDeviceBlocked(seqn, attributes);
}

void MainWindow::DeviceRejected(quint32 seqn, const std::map<std::string, std::string>& attributes, bool rule_match, quint32 rule_seqn)
{
  emit uiDeviceRejected(seqn, attributes);
}

void MainWindow::IPCConnected()
{
  emit uiConnected();
}

void MainWindow::IPCDisconnected()
{
  emit uiDisconnected();
}
