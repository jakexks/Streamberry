#include <QHostAddress>
#include <QList>
#include <QUdpSocket>
#include <iostream>
#include "libraryrequester.h"
#include "libraryreceiver.h"
#include "librarysender.h"
#include "sbexception.h"
#include "networking.h"

LibraryRequester::LibraryRequester(Database &datab): db(datab)
{
    QList<QString> gettinglibraries;
    udpsocket = new QUdpSocket(this);
    udpsocket->bind(45455, QUdpSocket::ShareAddress);
    connect(udpsocket, SIGNAL(readyRead()), this, SLOT(processNetworkActivity()));

}

void LibraryRequester::getLibrary(QHostAddress theirip, QString theirid, QString dblastupdate)
{
    if(!gettinglibraries.contains(theirid))
    {
        gettinglibraries.append(theirid);
        LibraryReceiver lr = LibraryReceiver(db);
        lr.receive();
        gettinglibraries.removeOne(theirid);
    }
}

void LibraryRequester::sendLibrary(QHostAddress theirip, QString theirid, QString dblastupdate)
{
    LibrarySender ls = LibrarySender(db);
    ls.send(dblastupdate.toInt(), theirip, theirid);
}

void LibraryRequester::processNetworkActivity()
{
    while (udpsocket->hasPendingDatagrams())
    {
        try
        {
            QByteArray datagram;
            datagram.resize(udpsocket->pendingDatagramSize());
            udpsocket->readDatagram(datagram.data(), datagram.size());
            QString datastring = (QString) datagram.data();
            networking n;
            QString id = n.parsebeacon(datastring, networking::uid);
            QString myid = n.getuniqid();
            if (n.parsebeacon(datastring, networking::beaconHeader) == "STREAMLIBRARY")
            {
                // Checks that beacon is not our own
                if (id != myid)
                {
                    QString dbtimestamp = n.parsebeacon(datastring, networking::timestamp);
                    LibrarySender ls = LibrarySender::LibrarySender(db);
                    ls.send(dbtimestamp.toInt(), QHostAddress::QHostAddress(n.parsebeacon(datastring, networking::ip)), id);
                }
            }
        }
        catch (SBException e)
        {
            std::cerr << e.getException().toStdString();
        }
    }
}
