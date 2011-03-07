#ifndef QUITMANAGER_H
#define QUITMANAGER_H
#include "beaconreceiver.h"
#include "beaconsender.h"

class QuitManager : public QObject
{
    Q_OBJECT
public:
    QuitManager();
public slots:
    void quitSlot();
};

#endif // QUITMANAGER_H
