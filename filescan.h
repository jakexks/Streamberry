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

//controls scanning of directories and adding result to database
class Filescan: public QObject
{
    Q_OBJECT
public:
  //This function instantiates a new Filescan class and initialises it's connection with the database
   Filescan(Database &datab);
   ~Filescan();
   //If this bool is set to true, then the Filescanner is currently scanning and can't be interfaced with.
   bool currentlyScanning();

public slots:
   //When a signal is emitted to this slot then the file scan begins
   void scan();

signals:
   //This signal is emitted when a filescan is completed and triggers the library update
   void finishedFileScan();

private:
   //This function takes a QList of tags as returned by the FileMeta class and checks them
   //to make sure they're formatted correctely and escaped correctly. If the tags are empty
   //they are replaced with a default value
   QList<QString> checktags(QList<QString> tags, QString filename);

   //Scans a folder for media files and calls Addfiles for each one.
   int scanFolder(QDir path, QStringList Expaths, QString homeid);

   //Scans all the files within the folder. Each file has it's tags read by FileMeta, then checked by checktags
   //and is finally added to the batch list for processing
   void addFiles(QDir path, QString homeID);

   //Depreciated for adding single files
   void addFile(QString filepath, QString homeID);

   //Takes a filepath and the name of the Local libary (Nearly always "Local") and begins the filescan.
   //Once the scan is complete, runs through the batch and inserts it into the database en mass.
   Filescan(QString filepath, QString homeID);
   int ismedia(QFileInfo file);

   //Depreciated for checking duplicates
   int isalreadyindat(QDir file);
   int isdup(QDir file);

   //Stores a pointer to the database class
   Database& db;

   //Stores the name of the local table
   QString localTable;
   //Stores the name of the local table in
   QString localTableScan;
   //Holds an instance of the filemeta class used for reading the tags from files
   FileMeta file;
   //holds a list of all the filepaths to add to the library in batch
   QList<QStringList>* filestoadd;
   bool scanning;
};


#endif // FILESCAN_H
