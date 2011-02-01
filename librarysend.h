#ifndef LIBRARYSEND_H
#define LIBRARYSEND_H
#include "database.h"
#include <QHostAddress>

class librarySend
{
public:
    librarySend(int timestamp, QString, Database &datab);
    librarySend(int timestamp, QHostAddress receiversIP, Database &datab);
private:
    Database &db;
};

#endif // LIBRARYSEND_H
