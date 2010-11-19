#ifndef BEACONSENDER_H
#define BEACONSENDER_H
#include <QtNetwork>

class beaconsender
{
public:
    beaconsender();

private:
    QUdpSocket *udpSocket;
    QTimer timer(&this);
    int messageNo;
    void startBroadcasting();
    void broadcastDatagram();
};

#endif // BEACONSENDER_H
