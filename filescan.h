#ifndef FILESCAN_H
#define FILESCAN_H

#include "database.h"
#include <QtDebug>
#include <QtSql>
#include <QString>
#include <QFile>
#include <QDir>
#include <QList>
#include "sbexception.h"
#include "filemeta.h"
#include "crossplatform.h"

using namespace std;


class Filescan: public QObject
{
    Q_OBJECT
public:
   Filescan(Database &datab);
   ~Filescan();
   //Filescan(Database &datab): db(datab){}
   int build_new(); //reads the list of folders to be scanned out of the database and adds media files in that
                    // folder and it's subfolders to it's library. Returns 1 if successful
   //Same as the above function except cleans out the current database first
   int build_new_clean();

signals:
   void finishedFileScan();

private:
   QList<QString> checktags(QList<QString> tags, QString filename);
   Database& db;
   int scanFolder(QDir path, QStringList Expaths, QString homeid);
   void addFiles(QDir path, QString homeID);
   void addFile(QString filepath, QString homeID);
   Filescan(QString filepath, QString homeID);
   int ismedia(QFileInfo file);
   int isalreadyindat(QDir file);
   int isdup(QDir file);
   QString localTable;
   FileMeta file;
};


#endif // FILESCAN_H
