#include <QHostAddress>
#include <QList>
#include <QUdpSocket>
#include <iostream>
#include <QList>
#include "libraryrequester.h"
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
    connect(&nn,SIGNAL(messageReceived(QString)),this,SLOT(receiveRequest(QString)));
    qDebug() << "LibraryRequester initialised";
}

void LibraryRequester::getLibrary(QHostAddress theirip, QString theirid, QString dblastupdate)
{
    networking n;
    bool status = 1;

    if(!gettinglibraries.contains(theirid))
    {
        // Beacon structure is "STREAMLIBRARYREQ|<my ip>|<database timestamp>
        QString message = "STREAMLIBRARYREQ|";
        message += n.getmyip();
        message += "|";
        message += dblastupdate;
        status = nn.send(theirip,45455, message);
    }
}

void LibraryRequester::receiveRequest(QString message)
{
    if(message.startsWith("STREAMLIBRARY"))
    {
        qDebug() << "Library Request Received";
        networking n;
        QList<QString> parts = message.split('|',QString::KeepEmptyParts);
        QHostAddress theirip = QHostAddress(parts.at(1));
        QString changes = "STREAMCHANGES|";
        changes += n.getuniqid();
        changes += "|";
        changes += db.changesSinceTime(parts.at(2).toInt(),n.getuniqid());
        nn.send(theirip,45455,changes);
    }
    else if (message.startsWith("STREAMCHANGES"))
    {
        qDebug() << "Syncing Library";
        QList<QString> parts = message.split("|", QString::KeepEmptyParts);
        gettinglibraries.append(parts.at(1));
        QString query = parts.at(2);
        //qDebug() << query;
        QList<QString> queries = query.split("\x1D");
        try
        {
            db.query("BEGIN;");
            for(int i = 0; i < queries.length(); i++)
            {
                QString tempq = queries.at(i);
                tempq.resize(tempq.lastIndexOf(';')+1);
                db.query(tempq);
            }
            db.query("COMMIT;");
            db.setOnline(message.split("|").at(1),"1");
        }
        catch(SBException e)
        {
            qDebug() << "ERROR: " << e.getException();
        }
        gettinglibraries.removeAll(message.split("|").at(1));
    }
    else
    {
        qDebug() << "Rubbish received " << message;
    }
}
