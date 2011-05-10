#include "streamrequest.h"

StreamRequest::StreamRequest()
{
//    connect(&client, SIGNAL(connected()),this, SLOT(startSend()));
}

void StreamRequest::sendConnect()
{
    //connect(&client, SIGNAL(connected()),this, SLOT(startSend()));
}

void StreamRequest::send(QString ipaddress, quint16 port, QString message)
{
    toSend = message;
    QHostAddress addr(ipaddress);
    client.connectToHost(addr, port);

    //if connected
    if(client.waitForConnected(3500))
    {
        //send the data
        startSend();
        //make sure it sent
        client.waitForBytesWritten(1000);
        //close connection
        client.close();
    }
    else
    {
        qDebug() << "Count not connect to host " << ipaddress;
    }
}

void StreamRequest::startSend()
{
    client.write(toSend.toUtf8());
}

void StreamRequest::startServer()
{
    connect(&server, SIGNAL(newConnection()),this, SLOT(acceptConnection()));
    server.listen(QHostAddress::Any, 45459);
}

void StreamRequest::acceptConnection()
{
    inclient = server.nextPendingConnection();
    connect(inclient, SIGNAL(readyRead()),this, SLOT(read()));
}

void StreamRequest::read()
{
    //char buffer[1024];
    //qDebug() << inclient->bytesAvailable();
    //QString buf(inclient->readAll());
    QString buf = QString::fromUtf8(inclient->readAll());
    //inclient->readData(buffer, inclient->bytesAvailable());
    qDebug() << buf;
    emit messageReceived(buf);
    inclient->close();
}

StreamRequest::~StreamRequest()
{
//    client.close();
    server.close();
}
