QT       += core gui network serialport positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ./Drawing
INCLUDEPATH += ./Fabric
INCLUDEPATH += ./Objects
INCLUDEPATH += ./DataVault
INCLUDEPATH += ./Creator
INCLUDEPATH += ./Connections
INCLUDEPATH += ./Parsers


SOURCES += \
    DataVault/itemsstorage.cpp \
    Fabric/fabric.cpp \
    Objects/fixeditem.cpp \
    Objects/toweditem.cpp \
    main.cpp \
    mainwindow.cpp \
    Drawing/mygraphicview.cpp \
    Connections/connection.cpp \
    Connections/connection_com.cpp \
    Connections/connection_net.cpp \
    Parsers/nmeaparser.cpp \
    Parsers/p190_creator.cpp \
    Creator/connection_creator.cpp

HEADERS += \
    DataVault/itemsstorage.h \
    Fabric/fabric.h \
    Objects/fixeditem.h \
    Objects/toweditem.h \
    mainwindow.h \
    Drawing/mygraphicview.h \
    Connections/connection.h \
    Creator/connection_creator.h \
    Connections/connection_com.h \
    Connections/connection_net.h \
    Parsers/nmeaparser.h \
    Parsers/p190_creator.h

FORMS += \
    mainwindow.ui \
    Creator/connection_creator.ui \
    Connections/connection.ui \
    Connections/connection_com.ui \
    Connections/connection_com.ui \
    Connections/connection_net.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
