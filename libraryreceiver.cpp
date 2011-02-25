#include <iostream>
#include "libraryreceiver.h"
#include <QUdpSocket>
#include "networking.h"
#include "database.h"
#include "sbexception.h"

LibraryReceiver::LibraryReceiver(Database &datab) : db(datab)
{
}

int LibraryReceiver::receive()
{
    networking n;
    QString data = n.receive(5758);
    QList<QString> queries = data.split('\x1D',QString::SkipEmptyParts);

    try
    {
        for(int i = 0; i < queries.length(); i++)
        {
           qDebug() << data;
           db.query(data);
        }
    }
    catch(SBException e)
    {
        std::cerr << e.getException().toStdString();
        return 1;
    }
    return 0;
}
