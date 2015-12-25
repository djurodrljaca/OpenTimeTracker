QT += core sql
QT -= gui

CONFIG += c++11

TARGET = Server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp \
    src/Database.cpp \
    src/Event.cpp \
    src/EventChangeLogItem.cpp \
    src/UserGroup.cpp \
    src/User.cpp \
    src/UserMapping.cpp \
    src/TimeTracker.cpp

HEADERS += \
    src/Database.hpp \
    src/Event.hpp \
    src/EventChangeLogItem.hpp \
    src/UserGroup.hpp \
    src/User.hpp \
    src/UserMapping.hpp \
    src/TimeTracker.hpp

RESOURCES += \
    qrc/database.qrc
