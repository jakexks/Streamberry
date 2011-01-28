#include "beaconreceiver.h"
#include "database.h"
#include "utilities.h"
#include "networking.h"
#include <QDebug>

// Constructor, needs no arguments
beaconreceiver::beaconreceiver(Database &datab): db(datab)
{
}

// Continually processes received datagrams by calling processPendingDatagrams
void beaconreceiver::run()
{
    qDebug() << udpSocket.bind(QHostAddress::LocalHost, 45454);
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
        qDebug() << datagram.data();
        QStringList beaconPart = datastring.split("|");
        QString id = beaconPart.takeAt(1), dbTimeStamp = beaconPart.takeAt(2);
        checkID(id, dbTimeStamp);
    }
}

// Checks whether a machine has been seen before, updates the timestamp if so and adds it to the list if not
// Also detects whether the library needs updating and alerts the library receiver if so
void beaconreceiver::checkID(QString id, QString dbTimeStamp)
{
    int stamp;
    if ((stamp = onlineMachines.value(id)) == 0)
    {
        //whatever needs to be done when a new machine is seen
        //TODO: tell the db that the machine is online
        //get the timestamp of last update from db
        if((QString::compare(db.lastUpdate(id), dbTimeStamp)) != 0)
        {
            //TODO: if receiver isn't busy then tell library receive to request their library
        }

    }
    //need to check if timestamp is different and tell library receiver to request library if so
    onlineMachines.insert(id, Utilities::getCurrentTimestamp());
}
