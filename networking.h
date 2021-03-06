#ifndef NETWORKING_H
#define NETWORKING_H
#include <QString>
#include <QNetworkInterface>
#include <QList>
#include <QTcpServer>
#include <QTcpSocket>

//generic networking controls and abilities
class networking
{
public:
    networking();
    static QString getuniqid();
    QString getmyip();
    QByteArray receive(int port);
    void send(QHostAddress, quint16, QByteArray);
    void udpSend(QHostAddress, quint16, QByteArray);
    static QString parsebeacon(QString, int);
    enum beaconFields { beaconHeader, uid, timestamp, ip };
};

#endif // NETWORKING_H
