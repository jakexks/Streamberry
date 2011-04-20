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

    Utilities util(execpath);
    Database db;

    Filescan fs(db);
    QThread fsthread(&a);
    fs.moveToThread(&fsthread);
    fsthread.start();

    if(db.getSetting("FirstRun") == "")
    {
        FirstRunWizard wizard(db, fs);
        wizard.show();
        int cancel = wizard.exec();
        if(cancel == 0)
        {
            fsthread.quit();
            fsthread.wait();
            return 0;
        }
        db.storeSetting("FirstRun", "1");
    }

    Player player;

    BeaconSender bs(db);
    QThread bsthread(&a);
    bs.moveToThread(&bsthread);
    bsthread.start();

    BeaconReceiver br(db);
    QThread brthread(&a);
    br.moveToThread(&brthread);
    brthread.start();

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
