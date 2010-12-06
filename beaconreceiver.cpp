#include "beaconreceiver.h"
#include <QDebug>

beaconreceiver::beaconreceiver()
{
}

void beaconreceiver::run()
{
    udpSocket.bind(45454, QUdpSocket::ShareAddress);
    qDebug() << "Test";
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
