#ifndef BEACONRECEIVER_H
#define BEACONRECEIVER_H
#include <QHash>
#include <QtNetwork>
#include <QUdpSocket>
#include "database.h"

Q_DECLARE_METATYPE(QHostAddress)

//Deals with incoming beacons
class BeaconReceiver : public QObject
{
    Q_OBJECT
public:
    //Constructs a beacon receiver object, takes a reference to the database
    BeaconReceiver(Database &datab);
signals:
    //Indicates that a library is out of sync and contains the information needed to update
    void getLibrary(QHostAddress theirip, QString theirid, QString dblastupdate);
private slots:
    //Processes any incoming streambeacons
    void processPendingDatagrams();
    //Removes all machines that have timed out
    void removeOfflineMachines();
private:
    //Creates a hash table that maps a string to an int
    QHash<QString, int> onlinemachines;
    Database &db;
    QString myid;
    //Checks that the BeaconReceiver knows the machine is online and that we have the most recent version of their library
    void checkID(QString id, QString dbtimestamp, QHostAddress theirip);
    QUdpSocket *udpsocket;
    QTimer *timer;
};

#endif // BEACONRECEIVER_H
