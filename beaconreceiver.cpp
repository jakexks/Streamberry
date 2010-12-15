#include "beaconreceiver.h"
#include "database.h"
#include "utilities.h"
#include "networking.h"
#include <QDebug>

// Constructor, needs no arguments
beaconreceiver::beaconreceiver()
{
}

// Continually processes received datagrams by calling processPendingDatagrams
void beaconreceiver::run()
{
    udpSocket.bind(QHostAddress::Broadcast, 45454, QUdpSocket::ShareAddress);
    while (true)
    {
        // Beacon structure is "STREAMBEACON|<unique ID>|<timestamp>|<ip address>
        if(udpSocket.hasPendingDatagrams())
            processPendingDatagrams();
    }
}

// INCOMPLETE

void beaconreceiver::processPendingDatagrams()
{
    while (udpSocket.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket.pendingDatagramSize());
        udpSocket.readDatagram(datagram.data(), datagram.size());
        QString datastring = (QString) datagram.data();
        qDebug() << datastring.split('|')[0];
        qDebug() << datagram.data();
    }
}

// Checks whether a machine has been seen before, updates the timestamp if so and adds it to the list if not
void beaconreceiver::checkID(QString id)
{
    int stamp;

    if ((stamp = onlineMachines.value(id)) != 0)
    {

    }
    onlineMachines.insert(id, getCurrentTimestamp());
}
