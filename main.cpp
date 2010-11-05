#include <QtGui/QApplication>
#include <QDir>
#include "mainwindow.h"
#include "database.h"
#include "sbexception.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Database db;

    return a.exec();
}
