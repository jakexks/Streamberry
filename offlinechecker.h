#ifndef OFFLINECHECKER_H
#define OFFLINECHECKER_H
#include <QThread>
#include <QHash>
#include "beaconreceiver.h"
#include "database.h"

class offlinechecker : public QThread
{
public:
    offlinechecker(beaconreceiver &br, Database &datab);
protected:
    void run();
private:
    void detectOfflineMachines();
    beaconreceiver &rec;
    Database &db;
};

#endif // OFFLINECHECKER_H
