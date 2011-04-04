#include "newnetworking.h"

NewNetworking::NewNetworking()
{
    //connect(&client, SIGNAL(connected()),this, SLOT(startSend()));
}

void NewNetworking::startSend()
{
    client.write(toSend.toUtf8());
}

bool NewNetworking::send(QHostAddress ip, quint16 port, QString message)
{
    toSend = message;
    client.connectToHost(ip, port);
    if (client.waitForConnected(3000)) {
        startSend();
        return true;
    }
    else {
        return false;
    }
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
    do {
        buffer.append(QString::fromUtf8(inclient->readAll()));
    }
    while(inclient->waitForReadyRead(1000));
    //buffer.resize(buffer.lastIndexOf(';') + 1);
    qDebug() << " BUFFER" << buffer << "ENDBUFFER";
    emit messageReceived(buffer);
    inclient->close();
    delete inclient;
}
