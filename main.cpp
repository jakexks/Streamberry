#include <qthread.h>
#include "threadtest.h"
#include "beaconsender.h"
#include "beaconreceiver.h"
#include <QDebug>
#include <mainwindow.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    beaconsender b;
    b.start();
    beaconreceiver r;
    r.start();
    return a.exec();
}
