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
<<<<<<< HEAD
    database.cpp \
    sbexception.cpp \
    crossplatform.cpp \
    utilities.cpp \
    filescan.cpp


HEADERS  += mainwindow.h \
    database.h \
    sbexception.h \
    crossplatform.h \
    utilities.h \
    filescan.h
=======
    threadtest.cpp \
    beaconsender.cpp \
    beaconreceiver.cpp \
    networking.cpp

HEADERS  += mainwindow.h \
    threadtest.h \
    beaconsender.h \
    beaconreceiver.h \
    networking.h
>>>>>>> 66b5192a2bfd16325cf5a29f4ab4474081331483

FORMS    += mainwindow.ui

LIBS += -plugin -sql -sqlite

QT += sql
