#include "networking.h"
#include <QCryptographicHash>
#include <QStringList>
#include <QUdpSocket>
#include <iostream>

// Constructor, needs no arguments
networking::networking()
{
}

// Returns a string containing the unique ID of the current machine
QString networking::getuniqid()
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    QString s = "";
    for (int i = 0; i < interfaces.length(); i++)
    {
        s.append(interfaces[i].hardwareAddress());
            //break;
    }
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(s.toUtf8());
    return hash.result().toHex();
}

// Returns a string containing the ip address of the current machine
QString networking::getmyip()
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    QString s = "";
    for (int i = 0; i < interfaces.length(); i++)
    {
        for (int j = 0; j < interfaces[i].allAddresses().length(); j++)
        {
            QString ip = interfaces[i].allAddresses()[j].toString();
            if (!ip.startsWith("fe80:", Qt::CaseInsensitive) && !ip.startsWith("127.") && !ip.startsWith("0:"))
            {
                return ip;
            }
        }
    }
    return NULL;
}

// Generic receive function. Waits until there is something to receive from the network, then returns a byte array containing all the data received
QByteArray networking::receive(int port)
{
    tcpServer = new QTcpServer();
    if(!tcpServer->listen(QHostAddress::Any,port))
    {
        qDebug() << "Could not Listen";
    }
    QByteArray buf;

    tcpServer->waitForNewConnection(-1);
    while(tcpServer->hasPendingConnections())
    {
        tcpServerConnection = tcpServer->nextPendingConnection();
        buf.resize(buf.size() + tcpServerConnection->bytesAvailable());
        buf.append(tcpServerConnection->readAll());
        tcpServerConnection->close();
    }
    tcpServer->close();
    tcpServer->~QTcpServer();
    return buf;
}

// Generic send function
void networking::send(QHostAddress host, quint16 port, QByteArray data)
{
    const int timeout = 5000;
    tcpClient = new QTcpSocket();
    tcpClient->connectToHost(host, port);
    if(tcpClient->waitForConnected(timeout))
    {
        tcpClient->write(data);
        tcpClient->close();
    }
    else
    {
        std::cerr << tcpClient->errorString().toStdString() << std::endl;
    }
}

// Unique ID parser
QString networking::parsebeacon(QString bc, int field)
{
    QStringList split = bc.split('|', QString::KeepEmptyParts, Qt::CaseInsensitive);
    return split.takeAt(field);
}
