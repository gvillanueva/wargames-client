QT += network websockets

TARGET = libwargames
TEMPLATE = lib

SOURCES += \
    jsonrpc/jsonrpcwebsocketclient.cpp \
    game.cpp \
    gamelistmodel.cpp \
    unit.cpp \
    user.cpp

HEADERS += \
    jsonrpc/jsonrpcwebsocketclient.h \
    game.h \
    gamelistmodel.h \
    unit.h \
    user.h

INCLUDEPATH += $$[QT_INSTALL_PREFIX]/include/qjsonrpc

win32:LIBS += -lqjsonrpc1
unix:LIBS += -lqjsonrpc

unix {
    target.path = /usr/lib
    INSTALLS += target
}
