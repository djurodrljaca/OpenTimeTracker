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
    src/Database/Database.cpp \
    src/Database/EventManagement.cpp \
    src/Database/SettingManagement.cpp \
    src/Database/UserManagement.cpp

HEADERS += \
    src/Event.hpp \
    src/EventChangeLogItem.hpp \
    src/UserGroup.hpp \
    src/User.hpp \
    src/UserMapping.hpp \
    src/TimeTracker.hpp \
    src/Database/Database.hpp \
    src/Database/EventManagement.hpp \
    src/Database/SettingManagement.hpp \
    src/Database/UserManagement.hpp

RESOURCES += \
    qrc/database.qrc
