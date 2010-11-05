#include <QtGui/QApplication>
#include "mainwindow.h"
#include "database.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Database db;

    return a.exec();
}
