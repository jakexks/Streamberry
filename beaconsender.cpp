#include <QtNetwork>
#include "beaconsender.h"

beaconsender::beaconsender()
{
    udpSocket = new QUdpSocket((QObject *)this);
    connect(timer, SIGNAL(timeout()), this, SLOT(broadcastDatagram()));

}

void beaconsender::startBroadcasting()
{
    timer->start(1000);
}

void beaconsender::broadcastDatagram()
{
    QByteArray datagram = "Bigot Bigot Bigot";
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             QHostAddress::Broadcast, 45454);
}
