#-------------------------------------------------
#
# Project created by QtCreator 2011-01-31T21:15:18
#
#-------------------------------------------------

QT += core gui network sql

LIBS += -plugin -sql -sqlite -lvlc.5 -L/Applications/VLC.app/Contents/MacOS/lib/

INCLUDEPATH += /Applications/VLC.app/Contents/MacOS/include/

TARGET = Streamberry
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp\
           database.cpp\
           utilities.cpp\
           sbexception.cpp\
           crossplatform.cpp\
           sbsearchbar.cpp \
           sidebarcontroller.cpp \
           albumpanel.cpp \
           topbarcontroller.cpp \
           librarycontroller.cpp \
           playbackcontroller.cpp \
           albumartdelegate.cpp \
    streamfile.cpp

HEADERS += mainwindow.h\
           database.h\
           utilities.h\
           sbexception.h\
           crossplatform.h\
           sbsearchbar.h\
           sidebarcontroller.h\
           albumpanel.h \
           sbsearchbar.h \
           sidebarcontroller.h \
           albumpanel.h \
           topbarcontroller.h \
           librarycontroller.h \
           playbackcontroller.h \
           albumartdelegate.h \
    streamfile.h
