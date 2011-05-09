#ifndef STREAMREQUEST_H
#define STREAMREQUEST_H

#include <QtNetwork>
#include <QObject>

//Sends requests to remote machines to stream
class StreamRequest : public QObject
{
        Q_OBJECT;
private:
    QTcpSocket client;
    QTcpSocket *inclient;
    QTcpServer server;
    QString toSend;

public:
    void sendConnect();
    //Send the message to ipaddress, using given port
    void send(QString ipaddress, quint16 port, QString message);
    void startServer();
    StreamRequest();
    ~StreamRequest();
public slots:
    void startSend();
    void acceptConnection();
    void read();
signals:
    void messageReceived(QString message);
};

#endif // STREAMREQUEST_H
