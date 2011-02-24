#ifndef LIBRARYRECEIVER_H
#define LIBRARYRECEIVER_H
#include "database.h"

class LibraryReceiver
{
public:
    LibraryReceiver(Database &datab);
    int receive();
private:
    Database &db;
};

#endif // LIBRARYRECEIVER_H
