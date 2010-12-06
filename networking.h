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
    QString getuniqid();
    void receive();
    void send();
private:
    QTcpServer tcpServer;
    QTcpSocket tcpClient;
    QTcpSocket *tcpServerConnection;
};

#endif // NETWORKING_H
