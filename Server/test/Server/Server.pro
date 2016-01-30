#-------------------------------------------------
#
# Project created by QtCreator 2016-01-30T21:13:57
#
#-------------------------------------------------

QT       += network sql testlib
QT       -= gui

CONFIG += c++11

TARGET = tst_ServerTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../src/BreakTimeCalculator.hpp \
    ../../src/Client.hpp \
    ../../src/Event.hpp \
    ../../src/EventChangeLogItem.hpp \
    ../../src/Schedule.hpp \
    ../../src/Server.hpp \
    ../../src/TimeTracker.hpp \
    ../../src/User.hpp \
    ../../src/UserGroup.hpp \
    ../../src/UserMapping.hpp \
    \
    ../../src/Database/DatabaseManagement.hpp \
    ../../src/Database/EventManagement.hpp \
    ../../src/Database/ScheduleManagement.hpp \
    ../../src/Database/SettingsManagement.hpp \
    ../../src/Database/UserManagement.hpp

SOURCES += \
    tst_ServerTest.cpp \
    ../../src/BreakTimeCalculator.cpp \
    ../../src/Client.cpp \
    ../../src/Event.cpp \
    ../../src/EventChangeLogItem.cpp \
    ../../src/Schedule.cpp \
    ../../src/Server.cpp \
    ../../src/TimeTracker.cpp \
    ../../src/User.cpp \
    ../../src/UserGroup.cpp \
    ../../src/UserMapping.cpp \
    \
    ../../src/Database/DatabaseManagement.cpp \
    ../../src/Database/EventManagement.cpp \
    ../../src/Database/ScheduleManagement.cpp \
    ../../src/Database/SettingsManagement.cpp \
    ../../src/Database/UserManagement.cpp

RESOURCES += \
    ../../qrc/database.qrc

DEFINES += SRCDIR=\\\"$$PWD/\\\"

DESTDIR = build
OBJECTS_DIR = build
MOC_DIR = build
