#include "beaconreceiver.h"
#include <QDebug>

beaconreceiver::beaconreceiver()
{
}

void beaconreceiver::receive()
{
    udpSocket.bind(45454, QUdpSocket::ShareAddress);
    qDebug() << "Test";
}

void beaconreceiver::run()
{
    while(true)
    {
        receive();
    }
}
