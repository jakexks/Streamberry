#include <QtGui/QApplication>
#include "mainwindow.h"
#include "threadtest.h"

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    //return a.exec();

    threadtest *t1 = new threadtest(1);
    threadtest *t2 = new threadtest(2);
    threadtest *t3 = new threadtest(3);
    t1->start();
    t2->start();
    t3->start();
    sleep(20);
    return 0;
}
