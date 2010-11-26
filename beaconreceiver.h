#ifndef BEACONRECEIVER_H
#define BEACONRECEIVER_H
#include <QThread>
#include <QtNetwork>
#include <QUdpSocket>

class beaconreceiver : public QThread
{
public:
    beaconreceiver();
protected:
    void run();
private:
    QUdpSocket udpSocket;
    void receive();
    void processPendingDatagrams();
};

#endif // BEACONRECEIVER_H
