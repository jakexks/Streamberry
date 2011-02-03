#ifndef BEACONRECEIVER_H
#define BEACONRECEIVER_H
#include <QThread>
#include <QHash>
#include "database.h"

class BeaconReceiver : public QThread
{
    Q_OBJECT
public:
    BeaconReceiver(Database &datab);
signals:
    void getLibrary(QString uid, QString dbtimestamp);
protected:
    void run();
private:
    // Sets up the hash table for storing connected machines, the unique ID of the machine is used as the hash key and a timestamp of when the last beacon was received is the data
    QHash<QString, int> onlineMachines;
    Database &db;
};

#endif // BEACONRECEIVER_H
