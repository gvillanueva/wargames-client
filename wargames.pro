#-------------------------------------------------
#
# Project created by QtCreator 2015-11-15T11:59:53
#
#-------------------------------------------------

QT       += core gui network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = basic
TEMPLATE = app


SOURCES += main.cpp\
    unit.cpp \
    logindialog.cpp \
    wargamesapplication.cpp \
    user.cpp \
    maindialog.cpp \
    game.cpp \
    gamelistmodel.cpp \
    gamedialog.cpp \
    creategamedialog.cpp \
    jsonrpcwebsocketclient.cpp \
    lobbydialog.cpp

HEADERS  += \
    unit.h \
    logindialog.h \
    wargamesapplication.h \
    user.h \
    maindialog.h \
    game.h \
    gamelistmodel.h \
    gamedialog.h \
    creategamedialog.h \
    jsonrpcwebsocketclient.h \
    lobbydialog.h

FORMS    += \
    logindialog.ui \
    maindialog.ui \
    gamedialog.ui \
    creategamedialog.ui \
    lobbydialog.ui

INCLUDEPATH += $$(QTDIR)/include/qjsonrpc

win32:LIBS += -lqjsonrpc1
unix:LIBS += -lqjsonrpc
