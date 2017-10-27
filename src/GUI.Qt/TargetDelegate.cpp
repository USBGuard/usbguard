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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "TargetDelegate.h"
#include "DeviceModel.h"

#include "usbguard/Rule.hpp"

#include <QComboBox>

TargetDelegate::TargetDelegate(QObject* parent)
  : QStyledItemDelegate(parent)
{
}

QWidget* TargetDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  (void)option;
  (void)index;
  QComboBox* editor = new QComboBox(parent);
  editor->setFrame(false);
  editor->setAutoFillBackground(true);
  editor->addItem(tr("allow"), QString("allow"));
  editor->addItem(tr("block"), QString("block"));
  editor->addItem(tr("reject"), QString("reject"));
  return editor;
}

void TargetDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
  QComboBox* combobox = static_cast<QComboBox*>(editor);
  DeviceModelItem* item = static_cast<DeviceModelItem*>(index.internalPointer());
  QString value = QString::fromStdString(usbguard::Rule::targetToString(item->getRequestedTarget()));
  const int value_index = combobox->findData(value);

  if (value_index != -1) {
    combobox->setCurrentIndex(value_index);
  }
}

void TargetDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
  QComboBox* combobox = static_cast<QComboBox*>(editor);
  usbguard::Rule::Target target = usbguard::Rule::targetFromString(combobox->itemData(
        combobox->currentIndex()).toString().toStdString());
  DeviceModelItem* item = static_cast<DeviceModelItem*>(index.internalPointer());
  static_cast<DeviceModel*>(model)->updateRequestedTarget(item, target);
}

void TargetDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  (void)index;
  editor->setGeometry(option.rect);
}

/* vim: set ts=2 sw=2 et */
