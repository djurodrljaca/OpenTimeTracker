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
    ../../src/Database.hpp \
    ../../src/UserInfo.hpp
SOURCES += \
    tst_DatabaseTest.cpp \
    ../../src/Database.cpp \
    ../../src/UserInfo.cpp

RESOURCES += \
    ../../qrc/database.qrc

DEFINES += SRCDIR=\\\"$$PWD/\\\"
