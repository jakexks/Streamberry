#include <QtGui/QApplication>
#include "mainwindow.h"
#include "utilities.h"
#include "database.h"
#include <QThread>
#include "streamfile.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Utilities util(argv[0]);
    Database db;

    MainWindow w(util, db);
    w.show();

    StreamFile stream;
    stream.addStream("/Users/Robbie/Music/Albums/Biffy Clyro - Only Revolutions/Biffy Clyro - Many Of Horror.mp3", "test", "127.0.0.1");
    qDebug() << stream.getStreamLength("Test");

    return a.exec();
}
