#include "beaconsender.h"
#include "networking.h"
beaconsender::beaconsender()
{

}

void beaconsender::send()
{
    udpSocket = new QUdpSocket();
    networking n;

    QByteArray datagram = "STREAMBEACON" + n.getuniqid();
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
