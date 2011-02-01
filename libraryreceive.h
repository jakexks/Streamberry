#ifndef LIBRARYRECEIVE_H
#define LIBRARYRECEIVE_H
#include "database.h"

class libraryReceive
{
public:
    libraryReceive(Database &datab);
    int receive();
private:
    Database &db;
    bool isBusy();
    bool busy;
};

#endif // LIBRARYRECEIVE_H
