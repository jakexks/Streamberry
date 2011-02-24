#ifndef LIBRARYSENDER_H
#define LIBRARYSENDER_H
#include <QHostAddress>
#include "database.h"

class LibrarySender
{
public:
    LibrarySender(Database &datab);
    int send(int timeStamp, QHostAddress sendTo, QString uniqid);
private:
    Database& db;
};

#endif // LIBRARYSENDER_H
