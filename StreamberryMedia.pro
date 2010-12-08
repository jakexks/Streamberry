#-------------------------------------------------
#
# Project created by QtCreator 2010-11-14T23:13:21
#
#-------------------------------------------------

QT       += core gui

TARGET = StreamberryMedia
TEMPLATE = app


SOURCES += main.cpp \
    vlc_on_qt.cpp \
    file_meta.cpp

HEADERS  += \
    vlc_on_qt.h \
    file_meta.h

LIBS += -lvlc
