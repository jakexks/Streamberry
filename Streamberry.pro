#-------------------------------------------------
#
# Project created by QtCreator 2011-02-02T10:29:35
#
#-------------------------------------------------

QT       += core gui network

TARGET = Streamberry
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    librarysender.cpp \
    libraryreceiver.cpp \
    libraryrequester.cpp \
    beaconreceiver.cpp \
    beaconsender.cpp \
    networking.cpp

HEADERS  += mainwindow.h \
    librarysender.h \
    libraryreceiver.h \
    libraryrequester.h \
    beaconreceiver.h \
    beaconsender.h \
    networking.h

FORMS    += mainwindow.ui
