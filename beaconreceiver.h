#ifndef BEACONRECEIVER_H
#define BEACONRECEIVER_H
#include <QThread>
#include <QtNetwork>
#include <QUdpSocket>

class beaconreceiver : public QThread
{
public:
    beaconreceiver();
protected:
    void run();
private:
    QUdpSocket udpSocket;
    void receive();
    void processPendingDatagrams();
    // Sets up the hash table for storing connected machines, the unique ID of the machine is used as the hash key and a timestamp of when the last beacon was received is the data
    QHash<QString, int> onlineMachines;
    void checkID(QString);
};

#endif // BEACONRECEIVER_H
