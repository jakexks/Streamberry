#include "beaconsender.h"
#include "networking.h"
#include "database.h"
#include <QtNetwork>
#include <QtCore>
#include <QDebug>

BeaconSender::BeaconSender(Database &datab): db(datab)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(send()));
    timer->start(5000);
}

// Sends a streambeacon to every machine on the local network
void BeaconSender::send(bool online)
{
    QUdpSocket *udpsocket = new QUdpSocket();
    udpsocket->bind(QHostAddress::Broadcast, 45454, QUdpSocket::ShareAddress);
    networking n;
    QString sendme = "";
    // Beacon structure is "STREAMBEACON|<unique ID>|<database timestamp>|<ip address>" or "STREAMOFFLINE..." for offline beacons
    if (online)
        sendme.append("STREAMBEACON|");
    else
        sendme.append("STREAMOFFLINE");
    sendme.append(n.getuniqid());
    sendme.append("|");
    // Gets the timestamp from the database, could be changed to get the timestamp from constructor and a slot for updates
    sendme.append(db.lastUpdate("-1"));
    sendme.append("|");
    sendme.append(n.getmyip());
    qDebug() << "sending " + sendme;
    QByteArray datagram = sendme.toUtf8();
    udpsocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 45454);
    delete udpsocket;
}

// Sends offline beacons to all machines on local network
void BeaconSender::sendOfflineBeacon()
{
    send(false);
    timer->stop();
}
