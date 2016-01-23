#-------------------------------------------------
#
# Project created by QtCreator 2015-12-25T14:02:30
#
#-------------------------------------------------

QT += testlib
QT -= gui

CONFIG += c++11

TARGET  = tst_TimeTrackerTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../src/BreakTimeCalculator.hpp \
    ../../src/Schedule.hpp \
    ../../src/TimeTracker.hpp

SOURCES += \
    tst_TimeTrackerTest.cpp \
    ../../src/BreakTimeCalculator.cpp \
    ../../src/Schedule.cpp \
    ../../src/TimeTracker.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

DESTDIR = build
OBJECTS_DIR = build
MOC_DIR = build
