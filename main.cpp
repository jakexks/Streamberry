#include <QtGui/QApplication>
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

    try {
        db.initialse();
        QString sql = "INSERT INTO main.test (name, value) VALUES ('Barry', 1);";
        db.query(sql);
    } catch (SBException e) {
        qDebug() << e.getException();
    }

    return a.exec();
}
