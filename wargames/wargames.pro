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
    ui/logindialog.cpp \
    wargamesapplication.cpp \
    ui/maindialog.cpp \
    ui/gamedialog.cpp \
    ui/creategamedialog.cpp \
    ui/lobbydialog.cpp \
    ui/gamelistdialog.cpp

HEADERS  += \
    ui/logindialog.h \
    wargamesapplication.h \
    ui/maindialog.h \
    ui/gamedialog.h \
    ui/creategamedialog.h \
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

RESOURCES += \
    chess.qrc

DISTFILES += \
    install/main.nsi

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libwargames/release/ -llibwargames
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libwargames/debug/ -llibwargames
else:unix: LIBS += -L$$OUT_PWD/../libwargames/ -llibwargames

INCLUDEPATH += $$PWD/../libwargames
INCLUDEPATH += $$[QT_INSTALL_PREFIX]/include/qjsonrpc
DEPENDPATH += $$PWD/../libwargames
win32:LIBS += -lqjsonrpc1
unix:LIBS += -lqjsonrpc

