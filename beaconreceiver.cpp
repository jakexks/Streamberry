#include "beaconreceiver.h"
#include "database.h"
#include "utilities.h"
#include "networking.h"
#include <QDebug>

// Constructor, needs no arguments
beaconreceiver::beaconreceiver(Database &datab): db(datab)
{
    myID = networking::getuniqid();
}

// Continually processes received datagrams by calling processPendingDatagrams
void beaconreceiver::run()
{
    qDebug() << udpSocket.bind(45454, QUdpSocket::ShareAddress);
    while (true)
    {
        // Beacon structure is "STREAMBEACON|<unique ID>|<timestamp>|<ip address>
        if(udpSocket.hasPendingDatagrams())
        {
            qDebug() << "HERE";
            processPendingDatagrams();
        }

    }
}

// Takes an unread datagram from the updSocket and passes the unique ID and the database timestamp to checkID
void beaconreceiver::processPendingDatagrams()
{
    while (udpSocket.hasPendingDatagrams())
    {
        //check datagram begins with STREAMBEACON
        //check beacon received is not own
        QByteArray datagram;
        datagram.resize(udpSocket.pendingDatagramSize());
        udpSocket.readDatagram(datagram.data(), datagram.size());
        QString datastring = (QString) datagram.data();
        qDebug() << datagram.data();
        if (networking::parsebeacon(datastring, networking::beaconHeader) == "STREAMBEACON")
        {
            QString id = networking::parsebeacon(datastring, networking::uid);
            if (id != myID)
            {
                QString dbTimeStamp = networking::parsebeacon(datastring, networking::timestamp);
                checkID(id, dbTimeStamp);
            }
        }
    }
}

// Checks whether a machine has been seen before, updates the timestamp if so and adds it to the list if not
// Also detects whether the library needs updating and alerts the library receiver if so
void beaconreceiver::checkID(QString id, QString dbTimeStamp)
{
    int stamp;
    if ((stamp = onlineMachines.value(id)) == 0)
    {
        //TODO: tell the db that the machine is online

    }
    // Checks whether the receiving machine has the most recent version of the senders' library and requests an update if not
    if ((QString::compare(db.lastUpdate(id), dbTimeStamp)) != 0)
    {
        //TODO: if receiver isn't busy then tell library receive to request their library
    }
    onlineMachines.insert(id, Utilities::getCurrentTimestamp());
}
