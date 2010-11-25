#ifndef BEACONSENDER_H
#define BEACONSENDER_H
#include <QThread>
#include <QtNetwork>

class beaconsender : public QThread
{
public:
    beaconsender();
    void send();
protected:
    void run();
private:
    QUdpSocket *udpSocket;
};

#endif // BEACONSENDER_H
