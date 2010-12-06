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
        if(QString::compare(interfaces[i].humanReadableName(), "lo") == 0)
        {
            // test on windows
        }
        else
        {
            s.append(interfaces[i].hardwareAddress());
            break;
        }
    }
    return s;
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
