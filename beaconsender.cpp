#include "beaconsender.h"
#include "networking.h"

// Constructor, needs no arguments
beaconsender::beaconsender()
{
}

// Sends a beacon to every machine on the local network
void beaconsender::send()
{
    udpSocket = new QUdpSocket();
    networking n;
    QString sendme = "";
    // Beacon structure is "STREAMBEACON|<unique ID>|<ip address>
    sendme.append("STREAMBEACON|");
    sendme.append(n.getuniqid());
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
