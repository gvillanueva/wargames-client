#-------------------------------------------------
#
# Project created by QtCreator 2015-11-15T11:59:53
#
#-------------------------------------------------

QT       += core gui network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wargames
TEMPLATE = app


SOURCES += main.cpp\
    unit.cpp \
    ui/logindialog.cpp \
    wargamesapplication.cpp \
    user.cpp \
    ui/maindialog.cpp \
    game.cpp \
    gamelistmodel.cpp \
    ui/gamedialog.cpp \
    ui/creategamedialog.cpp \
    jsonrpc/jsonrpcwebsocketclient.cpp \
    ui/lobbydialog.cpp \
    ui/gamelistdialog.cpp

HEADERS  += \
    unit.h \
    ui/logindialog.h \
    wargamesapplication.h \
    user.h \
    ui/maindialog.h \
    game.h \
    gamelistmodel.h \
    ui/gamedialog.h \
    ui/creategamedialog.h \
    jsonrpc/jsonrpcwebsocketclient.h \
    ui/lobbydialog.h \
    ui/gamelistdialog.h

FORMS    += \
    ui/logindialog.ui \
    ui/maindialog.ui \
    ui/gamedialog.ui \
    ui/creategamedialog.ui \
    ui/lobbydialog.ui \
    ui/gamelistdialog.ui

RC_FILE = wargames.rc

INCLUDEPATH += $$(QTDIR)/include/qjsonrpc

win32:LIBS += -lqjsonrpc1
unix:LIBS += -lqjsonrpc

RESOURCES += \
    chess.qrc

DISTFILES += \
    install/main.nsi
