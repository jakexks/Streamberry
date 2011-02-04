#include <QtGui/QApplication>
#include "mainwindow.h"
#include "database.h"
#include "beaconsender.h"

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

    return a.exec();
}
