#-------------------------------------------------
#
# Project created by QtCreator 2015-12-21T00:12:14
#
#-------------------------------------------------

QT += sql testlib
QT -= gui

CONFIG += c++11

TARGET  = tst_DatabaseTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../src/Database/DatabaseManagement.hpp \
    ../../src/Database/EventManagement.hpp \
    ../../src/Database/ScheduleManagement.hpp \
    ../../src/Database/SettingsManagement.hpp \
    ../../src/Database/UserManagement.hpp \
    ../../src/Event.hpp \
    ../../src/EventChangeLogItem.hpp \
    ../../src/Schedule.hpp \
    ../../src/User.hpp \
    ../../src/UserGroup.hpp \
    ../../src/UserMapping.hpp

SOURCES += \
    tst_DatabaseTest.cpp \
    ../../src/Database/DatabaseManagement.cpp \
    ../../src/Database/EventManagement.cpp \
    ../../src/Database/ScheduleManagement.cpp \
    ../../src/Database/SettingsManagement.cpp \
    ../../src/Database/UserManagement.cpp \
    ../../src/Event.cpp \
    ../../src/EventChangeLogItem.cpp \
    ../../src/Schedule.cpp \
    ../../src/User.cpp \
    ../../src/UserGroup.cpp \
    ../../src/UserMapping.cpp

RESOURCES += \
    ../../qrc/database.qrc

DEFINES += SRCDIR=\\\"$$PWD/\\\"

DESTDIR = build
OBJECTS_DIR = build
MOC_DIR = build
