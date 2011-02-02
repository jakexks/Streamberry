#include <QtGui/QApplication>
#include "mainwindow.h"
#include "utilities.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Utilities util(argv[0]);
    MainWindow w(util);
    w.show();

    return a.exec();
}
