#ifndef FILESCAN_H
#define FILESCAN_H

#include "filescan.h"
#include "database.h"
#include <QtDebug>
#include <QtSql>
#include <QString>
#include <QFile>
#include <QDir>
#include "sbexception.h"
#include "crossplatform.h"

using namespace std;


class filescan
{
public:
   filescan(Database &pointer);
   int build_new(); //reads the list of folders to be scanned out of the database and adds media files in that
                    // folder and it's subfolders to it's library. Returns 1 if successful

private:
   int scanFolder(QDir path, QStringList Expaths);
   int addFiles(QDir path);
   int ismedia(QDir file);
   int isalreadyindat(QDir file);
   int isdup(QDir file);
};


#endif // FILESCAN_H
