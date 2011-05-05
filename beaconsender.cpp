#include "beaconsender.h"
#include "networking.h"
#include "database.h"
#include <QtNetwork>
#include <QtCore>
#include <QDebug>

BeaconSender::BeaconSender(Database &datab): db(datab)
{
    networking n;
    myip = n.getmyip();
    timer = new QTimer(this);
    //connect the timer signal to the send slot
    connect(timer, SIGNAL(timeout()), this, SLOT(send()));
    //tell the timer to fire every 5 seconds
    timer->start(5000);
}

// Sends a streambeacon to every machine on the local network
void BeaconSender::send(bool online)
{
    QUdpSocket *udpsocket = new QUdpSocket();
    //bind the UDP socket to the port used for beacons
    udpsocket->bind(QHostAddress::Broadcast, 45454, QUdpSocket::ShareAddress);
    networking n;
    QString sendme = "";
    // Beacon structure is "STREAMBEACON|<unique ID>|<database timestamp>|<ip address>" or "STREAMOFFLINE..." for offline beacons
    if (online)
        sendme.append("STREAMBEACON|");
    else
        sendme.append("STREAMOFFLINE|");
    sendme.append(n.getuniqid());
    sendme.append("|");
    //get the timestamp from the database and add it to the beacon if not going offline
    if(online) sendme.append(db.lastUpdate("Local"));
    sendme.append("|");
    sendme.append(myip);
    qDebug() << "sending " + sendme;
    QByteArray datagram = sendme.toUtf8();
    //send the beacon
    udpsocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 45454);
    delete udpsocket;
}

// Sends offline beacons to all machines on local network
void BeaconSender::sendOfflineBeacon()
{
    timer->stop();
    send(false);
    //kill the thread
    exit(0);
}
