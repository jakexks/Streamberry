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

    //DB test script start
    Database db;

    try {
        db.initialse();
        QString sql = "INSERT INTO main.test (name, value) VALUES ('Barry', 2);";
        db.query(sql);
    } catch (SBException e) {
        qDebug() << e.getException();
    }
    //DB test script end

    return a.exec();
}
