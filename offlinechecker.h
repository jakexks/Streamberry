#ifndef OFFLINECHECKER_H
#define OFFLINECHECKER_H
#include <QThread>
#include <QHash>
#include "beaconreceiver.h"

class offlinechecker : public QThread
{
public:
    offlinechecker(beaconreceiver &br, Database &datab);
protected:
    void run();
private:
    void detectOfflineMachines();
    beaconreceiver &rec;
};

#endif // OFFLINECHECKER_H
