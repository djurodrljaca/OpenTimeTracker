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
    src/Database.cpp

HEADERS += \
    src/UserInfo.hpp \
    src/UserGroupInfo.hpp \
    src/Database.hpp

RESOURCES += \
    qrc/database.qrc
