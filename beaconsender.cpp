#include "beaconsender.h"
#include "networking.h"
#include "database.h"

beaconsender::beaconsender(Database &datab): db(datab)
{
}

// Sends a beacon to every machine on the local network
void beaconsender::send()
{
    udpSocket = new QUdpSocket();
    udpSocket->bind(QHostAddress::Broadcast, 45454, QUdpSocket::ShareAddress);
    networking n;
    QString sendme = "";
    // Beacon structure is "STREAMBEACON|<unique ID>|<timestamp>|<ip address>
    sendme.append("STREAMBEACON|");
    sendme.append(n.getuniqid());
    sendme.append("|");
    sendme.append(db.lastUpdate("Local"));
    sendme.append("|");
    sendme.append(n.getmyip());
    QByteArray datagram = sendme.toUtf8();
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             QHostAddress::Broadcast, 45454);
    delete udpSocket;
}

// Repeatedly sends a beacon then sleeps for 5 seconds
void beaconsender::run()
{
    while(true)
    {
        send();
        sleep(5);
    }
}
