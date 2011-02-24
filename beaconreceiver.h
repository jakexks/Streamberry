#ifndef BEACONRECEIVER_H
#define BEACONRECEIVER_H
#include <QHash>
#include <QtNetwork>
#include <QUdpSocket>
#include "database.h"

class BeaconReceiver : public QObject
{
    Q_OBJECT
public:
    BeaconReceiver(Database &datab);
signals:
    void getLibrary(QHostAddress theirip, QString theirid, QString dblastupdate);
private slots:
    void processPendingDatagrams();
    void removeOfflineMachines();
protected:
private:
    // Sets up the hash table for storing connected machines, the unique ID of the machine is used as the hash key and a timestamp of when the last beacon was received is the data
    QHash<QString, int> onlinemachines;
    Database &db;
    QString myid;
    void checkID(QString id, QString dbtimestamp, QHostAddress theirip);
    QUdpSocket *udpsocket;
    QTimer *timer;
};

#endif // BEACONRECEIVER_H
