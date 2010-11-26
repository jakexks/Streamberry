#include <qthread.h>
#include "threadtest.h"
#include "beaconsender.h"
#include "beaconreceiver.h"
#include <QDebug>
#include <mainwindow.h>
#include <QApplication>
//#include "networking.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    w.show();

    //return a.exec();
    qDebug() << "Test main";
    //networking n;
    //n.getuniqid();
    beaconsender b;
    b.start();
    beaconreceiver r;
    r.start();
    return a.exec();
}
