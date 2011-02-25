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
    qDebug() << "LibraryRequester initialised";
}

void LibraryRequester::getLibrary(QHostAddress theirip, QString theirid, QString dblastupdate)
{
    if(!gettinglibraries.contains(theirid))
    {
        gettinglibraries.append(theirid);
        qDebug() << "Attempting to receive library from " << theirid;
        networking n;
        QString sendme = "STREAMLIBRARY|";
        sendme.append(n.getuniqid());
        sendme.append("|");
        sendme.append(dblastupdate);
        sendme.append("|");
        sendme.append(n.getmyip());
        qDebug() << "Requesting library from " << theirip ;
        n.send(theirip, 45455, sendme.toUtf8());
        LibraryReceiver lr = LibraryReceiver(db);
        qDebug() << "Before";
        lr.receive();
        qDebug() << "After";
        gettinglibraries.removeOne(theirid);
    }
}

void LibraryRequester::sendLibrary(QHostAddress theirip, QString theirid, QString dblastupdate)
{
    LibrarySender ls = LibrarySender(db);
    qDebug()<<"Sending my library to " << theirid << " " << theirip;
    ls.send(dblastupdate.toInt(), theirip, theirid);
}

void LibraryRequester::processNetworkActivity()
{
    qDebug() << "Received send request";
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
                if (id != myid)
                {
                    QString dbtimestamp = n.parsebeacon(datastring, networking::timestamp);
                    LibrarySender ls = LibrarySender::LibrarySender(db);
                    qDebug() << "Sending my library to " << id;
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
