QT       += core gui network serialport positioning core5compat
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ./Drawing
INCLUDEPATH += ./Fabric
INCLUDEPATH += ./Objects
INCLUDEPATH += ./DataVault
INCLUDEPATH += ./ConnectionCreator
INCLUDEPATH += ./Connections
INCLUDEPATH += ./Parsers
INCLUDEPATH += ./Service
INCLUDEPATH += ./Data
INCLUDEPATH += ./Data/Params
INCLUDEPATH += ./DataReader
INCLUDEPATH += ./Test


SOURCES += \
    Connections/connection_udp.cpp \
    Connections/depthsensors.cpp \
    Connections/sender.cpp \
    DataVault/itemsloader.cpp \
    DataVault/itemsstorage.cpp \
    Drawing/lordofgraphic.cpp \
    Drawing/sidegraphicview.cpp \
    Fabric/buoydialog.cpp \
    Fabric/fabric.cpp \
    Fabric/streamerdialog.cpp \
    Objects/buoy.cpp \
    Objects/fixeditem.cpp \
    Objects/source.cpp \
    Objects/streamer.cpp \
    Objects/toweditem.cpp \
    Parsers/coordinator.cpp \
    DataReader/segyreader.cpp \
    Service/logger.cpp \
    Service/postprocessor.cpp \
    Test/test_itemsloading.cpp \
    Test/test_postprocessing.cpp \
    main.cpp \
    mainwindow.cpp \
    Drawing/mygraphicview.cpp \
    Connections/connection.cpp \
    Connections/connection_com.cpp \
    Connections/connection_net.cpp \
    Parsers/nmeaparser.cpp \
    Parsers/p190_creator.cpp \
    ConnectionCreator/connection_creator.cpp

HEADERS += \
    Connections/connection_udp.h \
    Connections/depthsensors.h \
    Connections/sender.h \
    DataVault/CircularBuffer.h \
    DataVault/itemsloader.h \
    DataVault/itemsstorage.h \
    DataVault/structures.h \
    Drawing/lordofgraphic.h \
    Drawing/sidegraphicview.h \
    Fabric/buoydialog.h \
    Fabric/fabric.h \
    Fabric/streamerdialog.h \
    Objects/buoy.h \
    Objects/fixeditem.h \
    Objects/source.h \
    Objects/streamer.h \
    Objects/toweditem.h \
    Parsers/coordinator.h \
    Parsers/functions.h \
    DataReader/segyreader.h \
    Service/logger.h \
    Service/postprocessor.h \
    Test/test_itemsloading.h \
    Test/test_postprocessing.h \
    mainwindow.h \
    Drawing/mygraphicview.h \
    Connections/connection.h \
    ConnectionCreator/connection_creator.h \
    Connections/connection_com.h \
    Connections/connection_net.h \
    Parsers/nmeaparser.h \
    Parsers/p190_creator.h

FORMS += \
    mainwindow.ui \
    ConnectionCreator/connection_creator.ui \
    Connections/connection.ui \
    Connections/connection_com.ui \
    Connections/connection_com.ui \
    Connections/connection_net.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += resources.qrc
