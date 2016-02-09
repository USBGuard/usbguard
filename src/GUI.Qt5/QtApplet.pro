#-------------------------------------------------
#
# Project created by QtCreator 2015-03-13T19:18:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 0.2
TARGET = usbguard-applet-qt
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES +=\
    main.cpp\
    MainWindow.cpp \
    DeviceDialog.cpp

HEADERS +=\
    MainWindow.h \
    DeviceDialog.h

FORMS += MainWindow.ui \
    DeviceDialog.ui

unix: CONFIG += link_pkgconfig

RESOURCES += \
    icon.qrc

