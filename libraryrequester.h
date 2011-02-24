#ifndef LIBRARYREQUESTER_H
#define LIBRARYREQUESTER_H
#include <QThread>
#include <QHostAddress>
#include <QHash>
#include <QUdpSocket>
#include "database.h"

class LibraryRequester : public QObject
{
    Q_OBJECT
public:
    LibraryRequester(Database &datab);
public slots:
    void getLibrary(QHostAddress theirip, QString theirid, QString dblastupdate);
    void sendLibrary(QHostAddress theirip, QString theirid, QString dblastupdate);
private slots:
    void processNetworkActivity();
private:
    QList<QString> gettinglibraries;
    Database& db;
    QUdpSocket *udpsocket;
};

#endif // LIBRARYREQUESTER_H
