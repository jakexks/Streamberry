#ifndef LIBRARYREQUESTER_H
#define LIBRARYREQUESTER_H
#include <QThread>
#include <QHostAddress>
#include <QHash>
#include <QUdpSocket>
#include "database.h"
#include <QTcpServer>
#include "newnetworking.h"

class LibraryRequester : public QObject
{
    Q_OBJECT
public:
    LibraryRequester(Database &datab);
public slots:
    bool getLibrary(QHostAddress theirip, QString theirid, QString dblastupdate);
    //void sendLibrary(QHostAddress theirip, QString theirid, QString dblastupdate);
private slots:
    void receiveRequest(QString message);
private:
    QList<QString> gettinglibraries;
    Database& db;
    QTcpServer *server;
    NewNetworking nn;
};

#endif // LIBRARYREQUESTER_H
