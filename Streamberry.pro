#-------------------------------------------------
#
# Project created by QtCreator 2010-11-05T13:59:27
#
#-------------------------------------------------

QT       += core gui

TARGET = Streamberry
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp \
    sbexception.cpp

HEADERS  += mainwindow.h \
    database.h \
    sbexception.h

FORMS    += mainwindow.ui

LIBS += -plugin-sql-sqlite

QT += sql
