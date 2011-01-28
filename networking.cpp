#include "networking.h"
#include "sleeper.h"
#include <QCryptographicHash>

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
QByteArray networking::receive()
{
    tcpServer.listen(QHostAddress::Any,6567);
    QByteArray buf;
    while (!tcpServer.isListening() && !tcpServer.listen())
    {
        qDebug() << "Could not listen";
        return NULL;
    }
    while(!tcpServer.hasPendingConnections())
    {
        sleeper::msleep(500);
    }
    while(tcpServer.hasPendingConnections())
    {
        tcpServerConnection = tcpServer.nextPendingConnection();
        buf.resize(buf.size() + tcpServerConnection->bytesAvailable());
        buf.append(tcpServerConnection->readAll());
        tcpServerConnection->close();
    }
    tcpServer.close();
    return buf;
}

// Generic send function
void networking::send(QHostAddress host, quint16 port, QByteArray data)
{
    tcpClient.connectToHost(host, port);
    tcpClient.write(data);
}

enum networking::beaconFields { beaconHeader, uid, timestamp, ip };

// Unique ID parser
QString networking::parsebeacon(QString bc, int field)
{
    QStringList split = bc.split('|', QString::SkipEmptyParts, Qt::CaseInsensitive);
    return split[field];
}
