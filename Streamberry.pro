#-------------------------------------------------
#
# Project created by QtCreator 2011-01-31T21:15:18
#
#-------------------------------------------------

QMAKE_CFLAGS += -O1
QMAKE_CXXFLAGS += -O1
mac:QMAKE_LFLAGS += -framework Cocoa

QT += core gui network sql

LIBS += -Wl,-Bdynamic -lvlc
#LIBS += -lvlc
mac:LIBS += -L/Applications/VLC.app/Contents/MacOS/lib/
mac:INCLUDEPATH += /Applications/VLC.app/Contents/MacOS/include/

win32:LIBS += -L"C:\\Program files\\VideoLAN\\VLC"
win32:LIBS += -L"C:\\Program files (x86)\\VideoLAN\\VLC"
win32:INCLUDEPATH += "C:\\vlc-1.1.7\\include"
TARGET = Streamberry
TEMPLATE = app

RC_FILE = icon.rc


SOURCES +=  main.cpp\
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
            filemeta.cpp \
            libraryrequester.cpp \
            beaconreceiver.cpp \
            beaconsender.cpp \
            networking.cpp \
            playlistscontext.cpp\
            streamrequest.cpp \
            playbackprogress.cpp \
            playbackbutton.cpp \
            songinfo.cpp\
            newnetworking.cpp \
            volumeslider.cpp \
            settingsdialog.cpp \
            trackcontext.cpp \
            playlistdialogue.cpp \
            plsmartedit.cpp \
            plsmartnew.cpp \
            plnormalnew.cpp \
            addto.cpp \
            firstrunwizard.cpp \
            previewpane.cpp \
            albumartgetter.cpp \
            maxplaylists.cpp

mac:OBJECTIVE_SOURCES += player.mm
win32:SOURCES += player.cpp
unix:!mac { SOURCES += player.cpp }

HEADERS +=  mainwindow.h\
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
            filemeta.h \
            libraryrequester.h \
            beaconreceiver.h \
            beaconsender.h \
            networking.h \
            player.h \
            playlistscontext.h \
            streamrequest.h \
            playbackprogress.h \
            playbackbutton.h \
            songinfo.h\
            newnetworking.h \
            volumeslider.h \
            settingsdialog.h \
            trackcontext.h \
            playlistdialogue.h \
            plsmartedit.h \
            plsmartnew.h \
            plnormalnew.h \
            addto.h \
            firstrunwizard.h \
            previewpane.h \
            albumartgetter.h \
            maxplaylists.h
