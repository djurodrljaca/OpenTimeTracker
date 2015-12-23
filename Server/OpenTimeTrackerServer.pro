QT += core sql
QT -= gui

CONFIG += c++11

TARGET = Server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp \
    src/UserInfo.cpp \
    src/UserGroupInfo.cpp \
    src/Database.cpp \
    src/UserMappingInfo.cpp \
    src/Event.cpp \
    src/EventChangeLogItem.cpp

HEADERS += \
    src/UserInfo.hpp \
    src/UserGroupInfo.hpp \
    src/Database.hpp \
    src/UserMappingInfo.hpp \
    src/Event.hpp \
    src/EventChangeLogItem.hpp

RESOURCES += \
    qrc/database.qrc
