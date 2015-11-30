#-------------------------------------------------
#
# Project created by QtCreator 2015-11-15T11:59:53
#
#-------------------------------------------------

QT       += core gui network

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
    gamedialog.cpp

HEADERS  += \
    unit.h \
    logindialog.h \
    wargamesapplication.h \
    user.h \
    maindialog.h \
    game.h \
    gamelistmodel.h \
    gamedialog.h

FORMS    += \
    logindialog.ui \
    maindialog.ui \
    gamedialog.ui

LIBS += -lqjsonrpc1
