#ifndef NEWNETWORKING_H
#define NEWNETWORKING_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

class NewNetworking : public QObject
{
    Q_OBJECT
public:
    NewNetworking();
    void startServer(quint16 port);
    void send(QHostAddress ip, quint16 port, QString message);
signals:
    void messageReceived(QString message);
public slots:
    void startSend();
    void acceptConnection();
    void read();
private:
    QTcpSocket client;
    QTcpSocket *inclient;
    QTcpServer server;
    QString toSend;
};

#endif // NEWNETWORKING_H
