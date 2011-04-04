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
    QTcpServer tcpServer;
    if(!tcpServer.listen(QHostAddress::Any,port))
    {
        qDebug() << "Could not Listen";
    }
    QByteArray buf;

    if(!tcpServer.waitForNewConnection(3000))
    {
        qDebug() << "Could not receive";
    }
    while(tcpServer.hasPendingConnections())
    {
        QTcpSocket* tcpServerConnection = tcpServer.nextPendingConnection();
        buf.resize(buf.size() + tcpServerConnection->bytesAvailable());
        buf.append(tcpServerConnection->readAll());
        tcpServerConnection->close();
        delete tcpServerConnection;
    }

    tcpServer.close();
    return buf;
}

// Generic send function
void networking::send(QHostAddress host, quint16 port, QByteArray data)
{
    const int timeout = 5000;
    QTcpSocket tcpClient;
    tcpClient.connectToHost(host, port);
    if(tcpClient.waitForConnected(timeout))
    {
        tcpClient.write(data);
        tcpClient.close();
    }
    else
    {
        std::cerr << tcpClient.errorString().toStdString() << std::endl;
    }
}

void networking::udpSend(QHostAddress ip, quint16 port, QByteArray data)
{
    QUdpSocket udpsocket;
    udpsocket.bind(QHostAddress::Broadcast, 45454, QUdpSocket::ShareAddress);
    udpsocket.writeDatagram(data.data(), data.size(), ip, port);
}

// Unique ID parser
QString networking::parsebeacon(QString bc, int field)
{
    QStringList split = bc.split('|', QString::KeepEmptyParts, Qt::CaseInsensitive);
    return split.takeAt(field);
}
