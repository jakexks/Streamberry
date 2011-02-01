#include "libraryreceive.h"
#include "database.h"
#include "networking.h"
#include "sbexception.h"
#include <iostream>

libraryReceive::libraryReceive(Database &datab): db(datab)
{
    db.setOnline(networking::getuniqid(), "1");
    busy = FALSE;
}

bool libraryReceive::isBusy()
{
    return busy;
}

int libraryReceive::receive()
{
    networking n;
    busy = TRUE;
    QString libraryQuery = n.receive();
    QList<QString> queries = libraryQuery.split('\x1D',QString::SkipEmptyParts);
    try
    {
        for (int i = 0; i < queries.length(); i++)
        {
            db.query(queries.takeAt(i));
        }
    }
    catch (SBException e)
    {
        std::cerr << e.getException().toStdString();
        return 1;
    }
    busy = FALSE;
    return 0;
}
