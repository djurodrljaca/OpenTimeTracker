TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += \
    src/main.cpp \
    src/UserInfo.cpp

RESOURCES += qml/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    src/UserInfo.hpp
