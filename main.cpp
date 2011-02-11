#include <QtGui/QApplication>
#include "mainwindow.h"
#include "database.h"
#include "beaconsender.h"
#include "beaconreceiver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Database db = Database();

    BeaconSender *bs = new BeaconSender(db);
    QThread *bsthread = new QThread(&a);
    bs->moveToThread(bsthread);
    bsthread->start();

    BeaconReceiver *br = new BeaconReceiver(db);
    QThread *brthread = new QThread(&a);
    br->moveToThread(brthread);
    brthread->start();

    QObject::connect(&a, SIGNAL(aboutToQuit()), bs, SLOT(sendOfflineBeacon()));

    return a.exec();
}
