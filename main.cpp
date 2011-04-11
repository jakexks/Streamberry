#include <QtGui/QApplication>
#include "mainwindow.h"
#include "utilities.h"
#include "database.h"
#include <QThread>
#include "streamfile.h"
#include <QDebug>
#include "database.h"
#include "sbexception.h"
#include "filescan.h"
#include "playlist.h"
#include "filemeta.h"
#include "beaconsender.h"
#include "beaconreceiver.h"
#include "player.h"
#include <QCleanlooksStyle>
#include "firstrunwizard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef Q_WS_X11
    //fix styling on linux
    a.setStyle(new QCleanlooksStyle());
#endif


#ifdef Q_WS_WIN
    QString execpath = a.applicationDirPath();
#else
    QString execpath(argv[0]);
    execpath.resize(execpath.lastIndexOf('/'));
#endif

    //tests first run wizard on every run
    //FirstRunWizard wizard;
    //wizard.show();

    Utilities util(execpath);

    Database db;
    Player player;

    Filescan fs(db);
    QThread fsthread(&a);
    fs.moveToThread(&fsthread);
    fsthread.start();

    BeaconSender bs(db);
    QThread bsthread(&a);
    bs.moveToThread(&bsthread);
    bsthread.start();

    BeaconReceiver br(db);
    QThread brthread(&a);
    br.moveToThread(&brthread);
    brthread.start();

    //redundant now that it does it after quit?
    //QObject::connect(&a, SIGNAL(aboutToQuit()), bs, SLOT(sendOfflineBeacon()));

    MainWindow w(util, db, player, fs, a);

    w.show();

//    StreamFile stream(player);
//    //stream.addStream("/Users/Robbie/Music/Albums/Biffy Clyro - Only Revolutions/Biffy Clyro - Many Of Horror.mp3", "test", "127.0.0.1");
//    //qDebug() << stream.getStreamLength("test");


    int ret = a.exec();
    //send offline beacon
    bs.sendOfflineBeacon();
    //safely close all threads
    fsthread.quit();
    fsthread.wait();
    bsthread.quit();
    bsthread.wait();
    brthread.quit();
    brthread.wait();
    return ret;
}
