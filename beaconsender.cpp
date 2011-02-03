#include "beaconsender.h"
#include "networking.h"
#include <QtNetwork>
#include <QtCore>

BeaconSender::BeaconSender()
{
}

// Sends a streambeacon to every machine on the local network
void BeaconSender::send()
{
    qDebug() << "sending beacon";
    QUdpSocket *udpsocket = new QUdpSocket();
    udpsocket->bind(QHostAddress::Broadcast, 45454, QUdpSocket::ShareAddress);
    networking n;
    QString sendme = "";
    // Beacon structure is "STREAMBEACON|<unique ID>|<database timestamp>|<ip address>"
    sendme.append("STREAMBEACON|");
    sendme.append(n.getuniqid());
    sendme.append("|");
    sendme.append(""/*db.lastUpdate("-1"*/);
    sendme.append("|");
    sendme.append(n.getmyip());
    QByteArray datagram = sendme.toUtf8();
    udpsocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 45454);
    delete udpsocket;
}

//
void BeaconSender::run()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(send()));
    timer->start(5000);
}
