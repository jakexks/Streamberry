#include "beaconreceiver.h"
#include <QDebug>

// Constructor, needs no arguments
beaconreceiver::beaconreceiver()
{
}

// Sets up the hash table for storing connected machines, the unique ID of the machine is used as the hash key and a timestamp of when the last beacon was received is the data
//QHash<QString, (whatever we use for timestamp)> onlineMachines;

// Continually processes received datagrams by calling processPendingDatagrams
void beaconreceiver::run()
{
    //set up data structure for IDs
    udpSocket.bind(45454, QUdpSocket::ShareAddress);
    while (true)
    {
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
        //QString id = "";
        datagram.resize(udpSocket.pendingDatagramSize());
        udpSocket.readDatagram(datagram.data(), datagram.size());
        //change datagram.data into a QString (id)
        //checkID(id);
        qDebug() << datagram.data();
    }
}

// Checks whether a machine has been seen before, updates the timestamp if so and adds it to the list if not
/*void beaconreceiver::checkID(QString id)
{
    declare stamp;
    if ((stamp = onlineMachines.value(id)) != <default-constructed timestamp>)
    {
        //update timestamp
        onlineMachines.insert(id, <timestamp>);
    }
    else
    {
        //insert ID into data structure
        onlineMachines.insert(id, <timestamp>);
    }
}*/
