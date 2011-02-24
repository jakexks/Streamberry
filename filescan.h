#ifndef FILESCAN_H
#define FILESCAN_H

#include "database.h"
#include <QtDebug>
#include <QtSql>
#include <QString>
#include <QFile>
#include <QDir>
#include "sbexception.h"
#include "crossplatform.h"

using namespace std;


class Filescan: public QObject
{
public:
   //Filescan();
   Filescan(Database &datab);
   //Filescan(Database &datab): db(datab){}
   int build_new(); //reads the list of folders to be scanned out of the database and adds media files in that
                    // folder and it's subfolders to it's library. Returns 1 if successful
   //Same as the above function except cleans out the current database first
   int build_new_clean();

private:
   Database& db;
   int scanFolder(QDir path, QStringList Expaths, QString homeid);
   int addFiles(QDir path, QString homeID);
   int ismedia(QFileInfo file);
   int isalreadyindat(QDir file);
   int isdup(QDir file);
};


#endif // FILESCAN_H
