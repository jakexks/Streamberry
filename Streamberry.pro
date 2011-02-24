#-------------------------------------------------
#
# Project created by QtCreator 2011-01-31T21:15:18
#
#-------------------------------------------------

QT += core gui network sql

LIBS += -plugin -sql -sqlite -lvlc
mac:LIBS += -L/Applications/VLC.app/Contents/MacOS/lib/

mac:INCLUDEPATH += /Applications/VLC.app/Contents/MacOS/include/

win32:LIBS += -L"C:\Program files\VideoLAN\VLC"
win32:LIBS += -L"C:\Program files (x86)\VideoLAN\VLC"
win32:INCLUDEPATH += "C:\vlc-1.1.7\include"

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
           filescan.cpp \
           playlist.cpp \
           streamfile.cpp \
           filemeta.cpp

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
           filescan.h \
           playlist.h \
           streamfile.h \
           filemeta.h
