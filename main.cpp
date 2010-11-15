#include <QtGui/QApplication>
#include <QDir>
#include <QDebug>
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

    //testing storing settings
    try
    {
        db.storeSetting("Test1", "tvalue1");
        db.storeSetting("Test2", "tvalue2");
        db.storeSetting("Test1", "tvalue3");

        qDebug() << "Test1 = " << db.getSetting("Test1");
        //should return null
        qDebug() << "Not in database = " << db.getSetting("notthere");
    }
    catch(SBException e)
    {
        qDebug() << e.getException();
    }

    return a.exec();
}
