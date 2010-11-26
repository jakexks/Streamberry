#ifndef BEACONSEND_H
#define BEACONSEND_H

#include <QThread>

class BeaconSend : QThread
{
public:
    BeaconSend(QObject *parent = 0);

protected:

private:
    void BeaconSend::broadcastDatagram();

};

#endif // BEACONSEND_H
