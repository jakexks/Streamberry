#include <QtGui/QApplication>
#include "mainwindow.h"
#include "threadtest.h"
#include "beaconsender.h"
#include "networking.h"

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    //return a.exec();
    networking n;
    qDebug() << n.getuniqid();
    beaconsender b;
    b.start();
    while (true)
        sleep(5);
    return 0;
}
