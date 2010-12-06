#include "beaconsender.h"
#include "networking.h"
beaconsender::beaconsender()
{

}

void beaconsender::send()
{
    udpSocket = new QUdpSocket();
    networking n;
    QString sendme = "";
    sendme.append("STREAMBEACON|");
    sendme.append(n.getuniqid());
    sendme.append("|");
    sendme.append(n.getmyip());
    QByteArray datagram = sendme.toUtf8();
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             QHostAddress::Broadcast, 45454);
    delete udpSocket;
}

void beaconsender::run()
{
    while(true)
    {
        send();
        sleep(5);
    }
}
