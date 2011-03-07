#include "newnetworking.h"

NewNetworking::NewNetworking()
{
    connect(&client, SIGNAL(connected()),this, SLOT(startSend()));
}

void NewNetworking::startSend()
{
    client.write(toSend.toUtf8());
}

void NewNetworking::send(QHostAddress ip, quint16 port, QString message)
{
    toSend = message;
    client.connectToHost(ip, port);
}

void NewNetworking::startServer(quint16 port)
{
    connect(&server, SIGNAL(newConnection()),this, SLOT(acceptConnection()));
    server.listen(QHostAddress::Any, port);
}

void NewNetworking::acceptConnection()
{
    inclient = server.nextPendingConnection();
    connect(inclient, SIGNAL(readyRead()),this, SLOT(read()));
}

void NewNetworking::read()
{
    QString buffer;
    buffer += (inclient->readAll());
    //buffer.resize(buffer.lastIndexOf(';') + 1);
    //qDebug() << buffer;
    emit messageReceived(buffer);
    inclient->close();
}
