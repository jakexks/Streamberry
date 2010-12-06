#include "beaconreceiver.h"
#include "database.h"
#include "utilities.h"
#include "networking.h"
#include <QDebug>

beaconreceiver::beaconreceiver()
{
}

void beaconreceiver::run()
{
    udpSocket.bind(45454, QUdpSocket::ShareAddress);
    while (true)
    {
        if(udpSocket.hasPendingDatagrams())
            processPendingDatagrams();
    }
}

void beaconreceiver::processPendingDatagrams()
{
    while (udpSocket.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket.pendingDatagramSize());
        udpSocket.readDatagram(datagram.data(), datagram.size());
        qDebug() << datagram.data();
    }
}
