#ifndef LIBRARYSEND_H
#define LIBRARYSEND_H
#include "database.h"
class librarySend
{
public:
    librarySend(int timestamp, QString, Database &datab);
private:
    Database &db;
};

#endif // LIBRARYSEND_H
