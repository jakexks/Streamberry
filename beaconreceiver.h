#ifndef BEACONRECEIVER_H
#define BEACONRECEIVER_H
#include <QThread>
#include <QHash>

class BeaconReceiver : public QThread
{
public:
    BeaconReceiver(Database &datab);
    // Sets up the hash table for storing connected machines, the unique ID of the machine is used as the hash key and a timestamp of when the last beacon was received is the data
    QHash<QString, int> onlineMachines;
};

#endif // BEACONRECEIVER_H
