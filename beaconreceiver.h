#ifndef BEACONRECEIVER_H
#define BEACONRECEIVER_H
#include <QThread>
#include <QtNetwork>
#include <QUdpSocket>
#include <QHash>
#include "database.h"

class beaconreceiver : public QThread
{
public:
    beaconreceiver(Database &datab);
    // Sets up the hash table for storing connected machines, the unique ID of the machine is used as the hash key and a timestamp of when the last beacon was received is the data
    QHash<QString, int> onlineMachines;
protected:
    void run();
private:
    QUdpSocket udpSocket;
    Database &db;
    void receive();
    void processPendingDatagrams();
    void checkID(QString, QString);
};

#endif // BEACONRECEIVER_H
