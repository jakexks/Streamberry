#include "beaconsender.h"

beaconsender::beaconsender()
{

}

void beaconsender::send()
{
    udpSocket = new QUdpSocket();
    QByteArray datagram = "DICKS";
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
