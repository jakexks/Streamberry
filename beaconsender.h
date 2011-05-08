#ifndef BEACONSENDER_H
#define BEACONSENDER_H
#include "database.h"

//sends beacons on network at interval or when needed
class BeaconSender : public QObject
{
    Q_OBJECT
public:
    //Constructs a beacon sender object, takes a reference to the database
    BeaconSender(Database &datab);
public slots:
    //Sends a beacon to all machines on the LAN indicating that this machine is going offline
    void sendOfflineBeacon();
private slots:
    //Sends a beacon to all machines on the LAN, assumes normal beacon unless passed 'false'
    void send(bool online = true);
private:
    Database& db;
    QTimer *timer;
    QString myip;
};

#endif // BEACONSENDER_H
