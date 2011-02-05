#include "beaconreceiver.h"
#include "networking.h"
#include "sbexception.h"
#include "utilities.h"
#include <QtNetwork>
#include <QUdpSocket>

// Creates a beaconReceiver that will process all streambeacons received over the LAN
BeaconReceiver::BeaconReceiver(Database &datab) : db(datab)
{
    networking n;
    myid = n.getuniqid();
    udpsocket = new QUdpSocket(this);
    udpsocket->bind(45454, QUdpSocket::ShareAddress);
    connect(udpsocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

// Gets a datagram from the UDPsocket, decodes it and checks that it belongs to streamberry, takes appropriate action if so
void BeaconReceiver::processPendingDatagrams()
{
    while (udpsocket->hasPendingDatagrams())
    {
        try
        {
            QByteArray datagram;
            datagram.resize(udpsocket->pendingDatagramSize());
            udpsocket->readDatagram(datagram.data(), datagram.size());
            QString datastring = (QString) datagram.data();
            qDebug() << "received " + datastring;
            networking n;
            QString id = n.parsebeacon(datastring, networking::uid);
            // If normal beacon then check sender is in the hashtable and check their library
            if (n.parsebeacon(datastring, networking::beaconHeader) == "STREAMBEACON")
            {
                // Checks that beacon is not our own
                if (id != myid)
                {
                    QString dbtimestamp = n.parsebeacon(datastring, networking::timestamp);
                    checkID(id, dbtimestamp);
                }
            }
            // If offline beacon
            else if (n.parsebeacon(datastring, networking::beaconHeader) == "STREAMOFFLINE")
            {
                if (id != myid)
                {
                    // Sets the machine offline in the database and removes it from the hashtable
                    db.setOnline(id, "0");
                    onlinemachines.remove(id);
                    qDebug() << id + "is offline";
                }
            }
        }
        catch (SBException e)
        {
            qDebug() << e.getException();
        }
    }
}

// Checks that the BeaconReceiver knows the machine is online and that we have the most recent version of their library
void BeaconReceiver::checkID(QString id, QString dbtimestamp)
{
    try
    {
        int stamp = onlinemachines.value(id);
        if (db.lastUpdate(id) == dbtimestamp)
        {
            //TODO: tell library requester to get their library
        }
        // If the machine has been seen before, but just come online then tell the database that it is online
        else if (stamp == 0)
        {
            db.setOnline(id, "1");
        }
    }
    catch (SBException e)
    {
        // Adds machine to database as it has not been seen before
        //TODO: enter real user name rather than Gary
        QString username = "Gary";
        db.makeUser(dbtimestamp, QString::number(Utilities::getCurrentTimestamp()), id, username);
        //TODO: tell library requester to get their library
    }
    onlinemachines.insert(id, Utilities::getCurrentTimestamp());
}

// Iterates over the hashtable of online machines and checks for timeouts
void BeaconReceiver::removeOfflineMachines()
{

}
