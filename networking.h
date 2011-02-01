#ifndef NETWORKING_H
#define NETWORKING_H
#include <QString>
#include <QNetworkInterface>
#include <QList>
#include <QTcpServer>
#include <QTcpSocket>


class networking
{
public:
    networking();
    static QString getuniqid();
    QString getmyip();
    QByteArray receive();
    void send(QHostAddress, quint16, QByteArray);
    static QString parsebeacon(QString, int);
    enum beaconFields { beaconHeader, uid, timestamp, ip };
private:
    QTcpServer tcpServer;
    QTcpSocket tcpClient;
    QTcpSocket *tcpServerConnection;
};

#endif // NETWORKING_H
