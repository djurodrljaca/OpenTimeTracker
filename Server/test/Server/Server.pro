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
    ../../src/Database/DatabaseManagement.hpp \
    ../../src/Database/EventManagement.hpp \
    ../../src/Database/ScheduleManagement.hpp \
    ../../src/Database/SettingsManagement.hpp \
    ../../src/Database/UserManagement.hpp \
    \
    ../../src/Packets/KeepAliveRequestPacket.hpp \
    ../../src/Packets/KeepAliveRequestPacketReader.hpp \
    ../../src/Packets/KeepAliveRequestPacketWriter.hpp \
    ../../src/Packets/KeepAliveResponsePacket.hpp \
    ../../src/Packets/KeepAliveResponsePacketReader.hpp \
    ../../src/Packets/KeepAliveResponsePacketWriter.hpp \
    ../../src/Packets/Packet.hpp \
    ../../src/Packets/PacketReader.hpp \
    ../../src/Packets/PacketWriter.hpp \
    \
    ../../src/BreakTimeCalculator.hpp \
    ../../src/Client.hpp \
    ../../src/Event.hpp \
    ../../src/EventChangeLogItem.hpp \
    ../../src/PacketHandler.hpp \
    ../../src/Schedule.hpp \
    ../../src/Server.hpp \
    ../../src/TimeTracker.hpp \
    ../../src/User.hpp \
    ../../src/UserGroup.hpp \
    ../../src/UserMapping.hpp

SOURCES += \
    tst_ServerTest.cpp \
    \
    ../../src/Database/DatabaseManagement.cpp \
    ../../src/Database/EventManagement.cpp \
    ../../src/Database/ScheduleManagement.cpp \
    ../../src/Database/SettingsManagement.cpp \
    ../../src/Database/UserManagement.cpp \
    \
    ../../src/Packets/KeepAliveRequestPacket.cpp \
    ../../src/Packets/KeepAliveRequestPacketReader.cpp \
    ../../src/Packets/KeepAliveRequestPacketWriter.cpp \
    ../../src/Packets/KeepAliveResponsePacket.cpp \
    ../../src/Packets/KeepAliveResponsePacketReader.cpp \
    ../../src/Packets/KeepAliveResponsePacketWriter.cpp \
    ../../src/Packets/Packet.cpp \
    ../../src/Packets/PacketReader.cpp \
    ../../src/Packets/PacketWriter.cpp \
    \
    ../../src/BreakTimeCalculator.cpp \
    ../../src/Client.cpp \
    ../../src/Event.cpp \
    ../../src/EventChangeLogItem.cpp \
    ../../src/Schedule.cpp \
    ../../src/PacketHandler.cpp \
    ../../src/Server.cpp \
    ../../src/TimeTracker.cpp \
    ../../src/User.cpp \
    ../../src/UserGroup.cpp \
    ../../src/UserMapping.cpp

RESOURCES += \
    ../../qrc/database.qrc

DEFINES += SRCDIR=\\\"$$PWD/\\\"

DESTDIR = build
OBJECTS_DIR = build
MOC_DIR = build
