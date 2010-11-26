#include "beaconsend.h"
#include <QtNetwork>

BeaconSend::BeaconSend(QObject *parent = 0)
{

    udpSocket = new QUdpSocket(this);
    //! [0]
    messageNo = 1;
}

void BeaconSend::broadcastDatagram()
{
//! [1]
    QByteArray datagram = "Broadcast message " + QByteArray::number(messageNo);
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             QHostAddress::Broadcast, 45454);
//! [1]
    ++messageNo;
}
