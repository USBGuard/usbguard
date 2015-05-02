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
#include <DeviceManager.hpp>
#include <DeviceManagerHooks.hpp>
#include "DeviceManagerPrivate.hpp"

namespace usbguard {
  DeviceManager::DeviceManager(DeviceManagerHooks& hooks)
  {
    d_pointer = new DeviceManagerPrivate(*this, hooks);
    return;
  }

  DeviceManager::DeviceManager(const DeviceManager& rhs)
  {
    d_pointer = new DeviceManagerPrivate(*this, *rhs.d_pointer);
    return;
  }

  const DeviceManager& DeviceManager::operator=(const DeviceManager& rhs)
  {
    DeviceManagerPrivate* n_pointer = new DeviceManagerPrivate(*this, *rhs.d_pointer);
    delete d_pointer;
    d_pointer = n_pointer;
    return *this;
  }
  
  DeviceManager::~DeviceManager()
  {
    delete d_pointer;
    d_pointer = nullptr;
    return;
  }

  void DeviceManager::insertDevice(Pointer<Device> device)
  {
    d_pointer->insertDevice(device);
    return;
  }

  Pointer<Device> DeviceManager::removeDevice(uint32_t seqn)
  {
    return d_pointer->removeDevice(seqn);
  }

  PointerVector<Device> DeviceManager::getDeviceList()
  {
    return std::move(d_pointer->getDeviceList());
  }

  Pointer<Device> DeviceManager::getDevice(uint32_t seqn)
  {
    return d_pointer->getDevice(seqn);
  }

  void DeviceManager::DeviceInserted(Pointer<Device> device)
  {
    d_pointer->DeviceInserted(device);
    return;
  }

  void DeviceManager::DevicePresent(Pointer<Device> device)
  {
    d_pointer->DevicePresent(device);
    return;
  }

  void DeviceManager::DeviceRemoved(Pointer<Device> device)
  {
    d_pointer->DeviceRemoved(device);
    return;
  }

  void DeviceManager::DeviceAllowed(Pointer<Device> device)
  {
    d_pointer->DeviceAllowed(device);
    return;
  }

  void DeviceManager::DeviceBlocked(Pointer<Device> device)
  {
    d_pointer->DeviceBlocked(device);
    return;
  }

  void DeviceManager::DeviceRejected(Pointer<Device> device)
  {
    d_pointer->DeviceRejected(device);
    return;
  }
} /* namespace usbguard */

#if defined(__linux__)
# include "LinuxDeviceManager.hpp"
#endif

usbguard::Pointer<usbguard::DeviceManager> usbguard::DeviceManager::create(DeviceManagerHooks& hooks)
{
#if defined(__linux__)
  auto dm = usbguard::makePointer<usbguard::LinuxDeviceManager>(hooks);
#else
# error "No DeviceManager implementation available"
#endif
  return std::move(dm);
}
