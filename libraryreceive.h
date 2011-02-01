#ifndef LIBRARYRECEIVE_H
#define LIBRARYRECEIVE_H
#include "database.h"

class libraryReceive
{
public:
    libraryReceive(Database &datab);
private:
    Database &db;
};

#endif // LIBRARYRECEIVE_H
