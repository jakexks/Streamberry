#include <QHostAddress>
#include <QList>
#include <QUdpSocket>
#include <iostream>
#include "libraryrequester.h"
#include "libraryreceiver.h"
#include "librarysender.h"
#include "sbexception.h"
#include "networking.h"
#include "newnetworking.h"

LibraryRequester::LibraryRequester(Database &datab): db(datab)
{
//    server = new QTcpServer();
//    if(!server->listen(QHostAddress::Any,45455))
//    {
//        qDebug() << "Could not Listen (in processnetworkactivity)";
//    }
//    connect(server,SIGNAL(newConnection()),this,SLOT(processNetworkActivity()));
    nn.startServer(45455);
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
        nn.send(theirip, 45455, sendme);
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
    QByteArray buf;
    try
    {
        while(server->hasPendingConnections())
        {
            qDebug() << "Proceesing pending connection";
            QTcpSocket *connection = server->nextPendingConnection();
            buf.resize(buf.size() + connection->bytesAvailable());
            buf.append(connection->readAll());
            connection->close();
        }
        QString datastring = (QString) buf.data();
        qDebug() << "Datastring: " << datastring;
        networking n;
        QString id = n.parsebeacon(datastring, 0);
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
            else
            {
                qDebug() << "From myself";
            }
        }
    }
    catch (SBException e)
    {
        std::cerr << e.getException().toStdString();
    }

}
