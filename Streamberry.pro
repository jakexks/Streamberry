#-------------------------------------------------
#
# Project created by QtCreator 2010-11-05T13:59:27
#
#-------------------------------------------------

QT       += core gui network

TARGET = Streamberry
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp \
    sbexception.cpp \
    crossplatform.cpp \
    utilities.cpp \
    filescan.cpp \
    threadtest.cpp \
    beaconsender.cpp \
    beaconreceiver.cpp \
    networking.cpp


HEADERS  += mainwindow.h \
    database.h \
    sbexception.h \
    crossplatform.h \
    utilities.h \
    filescan.h \
    threadtest.h \
    beaconsender.h \
    beaconreceiver.h \
    networking.h

FORMS    += mainwindow.ui

LIBS += -plugin -sql -sqlite

QT += sql
