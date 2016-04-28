#-------------------------------------------------
#
# Project created by QtCreator 2015-11-15T11:59:53
#
#-------------------------------------------------

QT       += core gui network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = basic
TEMPLATE = app


SOURCES += src/main.cpp\
    src/unit.cpp \
    src/ui/logindialog.cpp \
    src/wargamesapplication.cpp \
    src/user.cpp \
    src/ui/maindialog.cpp \
    src/game.cpp \
    src/gamelistmodel.cpp \
    src/ui/gamedialog.cpp \
    src/ui/creategamedialog.cpp \
    src/jsonrpc/jsonrpcwebsocketclient.cpp \
    src/ui/lobbydialog.cpp \
    src/ui/gamelistdialog.cpp

HEADERS  += \
    src/unit.h \
    src/ui/logindialog.h \
    src/wargamesapplication.h \
    src/user.h \
    src/ui/maindialog.h \
    src/game.h \
    src/gamelistmodel.h \
    src/ui/gamedialog.h \
    src/ui/creategamedialog.h \
    src/jsonrpc/jsonrpcwebsocketclient.h \
    src/ui/lobbydialog.h \
    src/ui/gamelistdialog.h

FORMS    += \
    src/ui/logindialog.ui \
    src/ui/maindialog.ui \
    src/ui/gamedialog.ui \
    src/ui/creategamedialog.ui \
    src/ui/lobbydialog.ui \
    src/ui/gamelistdialog.ui

INCLUDEPATH += $$(QTDIR)/include/qjsonrpc

win32:LIBS += -lqjsonrpc1
unix:LIBS += -lqjsonrpc

RESOURCES += \
    chess.qrc

DISTFILES += \
    src/install/main.nsi
