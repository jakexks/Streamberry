#include "networking.h"
#include <iostream>

networking::networking()
{
}

QString networking::getuniqid()
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    QString s = "";
    for (int i = 0; i < interfaces.length(); i++)
    {
        qDebug() << interfaces[i].humanReadableName() << interfaces[i].hardwareAddress();
        if(interfaces[i].humanReadableName().startsWith("lo"))
        {
            // test on windows
        }
        else
        {
            s.append(interfaces[i].hardwareAddress());
            //break;
        }
    }
    return s;
}

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
        usleep(500);
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

void networking::send(QHostAddress host, quint16 port, QByteArray data)
{
    tcpClient.connectToHost(host, port);
    tcpClient.write(data);
}
