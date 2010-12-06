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
    threadtest.cpp \
    beaconsender.cpp \
    beaconreceiver.cpp \
    networking.cpp

HEADERS  += mainwindow.h \
    threadtest.h \
    beaconsender.h \
    beaconreceiver.h \
    networking.h

FORMS    += mainwindow.ui
