#include "beaconreceiver.h"
#include "networking.h"
#include "sbexception.h"
#include "utilities.h"
#include <QtNetwork>
#include <QUdpSocket>
#include <QHashIterator>
#include "libraryrequester.h"

// Creates a beaconReceiver that will process all streambeacons received over the LAN
BeaconReceiver::BeaconReceiver(Database &datab) : db(datab)
{
    networking n;
    myid = n.getuniqid();

    LibraryRequester *lr = new LibraryRequester(db);
    QThread *lrthread = new QThread(this);
    lr->moveToThread(lrthread);
    lrthread->start();

    connect(this, SIGNAL(getLibrary(QHostAddress,QString,QString)), lr, SLOT(getLibrary(QHostAddress,QString,QString)));

    // Creates a timer that tells the object when to check for timeouts
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(removeOfflineMachines()));
    timer->start(10000);
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
                    checkID(id, dbtimestamp, QHostAddress::QHostAddress(n.parsebeacon(datastring, networking::ip)));
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
                    qDebug() << id + " is offline";
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
void BeaconReceiver::checkID(QString id, QString dbtimestamp, QHostAddress theirip)
{
    try
    {
        int stamp = onlinemachines.value(id);
        if (db.lastUpdate(id) < dbtimestamp)
        {
            networking n;
            QString sendme = "STREAMLIBRARY|";
            sendme.append(n.getuniqid());
            sendme.append("|");
            sendme.append(dbtimestamp);
            sendme.append("|");
            sendme.append(n.getmyip());

            n.send(theirip, 45455, sendme.toUtf8());
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
        //TODO: enter real user name rather than Gary OR get the library requester to enter username as this would be more efficient
        QString username = "Gary Oak";
        db.makeUser(dbtimestamp, QString::number(Utilities::getCurrentTimestamp()), id, username);
        getLibrary(theirip, id, dbtimestamp);
    }
    onlinemachines.insert(id, Utilities::getCurrentTimestamp());
}

// Iterates over the hashtable of online machines and checks for timeouts
void BeaconReceiver::removeOfflineMachines()
{
    QHashIterator<QString, int> i(onlinemachines);
    while (i.hasNext())
    {
        i.next();
        if (Utilities::getCurrentTimestamp() - i.value() > 10)
        {
            // Set machine offline in the database
            db.setOnline(i.key(), "0");
            qDebug() << i.key() + " has timed out";
            // Remove the machine's ID from the hashtable of online machines
            onlinemachines.remove(i.key());
        }
    }
}
