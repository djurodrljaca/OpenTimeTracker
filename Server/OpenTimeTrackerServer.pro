QT += core sql
QT -= gui

CONFIG += c++11

TARGET = Server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp \
    src/Event.cpp \
    src/EventChangeLogItem.cpp \
    src/UserGroup.cpp \
    src/User.cpp \
    src/UserMapping.cpp \
    src/TimeTracker.cpp \
    src/Database/EventManagement.cpp \
    src/Database/UserManagement.cpp \
    src/Database/DatabaseManagement.cpp \
    src/Database/SettingsManagement.cpp

HEADERS += \
    src/Event.hpp \
    src/EventChangeLogItem.hpp \
    src/UserGroup.hpp \
    src/User.hpp \
    src/UserMapping.hpp \
    src/TimeTracker.hpp \
    src/Database/EventManagement.hpp \
    src/Database/UserManagement.hpp \
    src/Database/DatabaseManagement.hpp \
    src/Database/SettingsManagement.hpp

RESOURCES += \
    qrc/database.qrc
