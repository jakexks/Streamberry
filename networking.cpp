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
