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
        mainwindow.cpp \
    unit.cpp \
    logindialog.cpp \
    wargamesapplication.cpp \
    user.cpp

HEADERS  += mainwindow.h \
    unit.h \
    logindialog.h \
    wargamesapplication.h \
    user.h

FORMS    += mainwindow.ui \
    logindialog.ui

LIBS += -lqjsonrpc1
