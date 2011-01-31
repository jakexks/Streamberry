#-------------------------------------------------
#
# Project created by QtCreator 2011-01-31T21:15:18
#
#-------------------------------------------------

QT       += core gui network sql

LIBS += -plugin -sql -sqlite

TARGET = Streamberry
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        database.cpp\
        utilities.cpp\
        sbexception.cpp\
        crossplatform.cpp

HEADERS  += mainwindow.h\
            database.h\
            utilities.h\
            sbexception.h\
            crossplatform.h
