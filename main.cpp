#include <QtGui/QApplication>
#include "mainwindow.h"
#include "utilities.h"
#include "database.h"
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Utilities util(argv[0]);
    Database db;

    QThread *dbthread = new QThread();
    db.moveToThread(dbthread);
    dbthread->start();

    MainWindow w(util, db);
    w.show();

    return a.exec();
}
