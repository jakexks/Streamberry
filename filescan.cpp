#include "filescan.h"
#include <QtDebug>
#include <QtSql>
#include <QString>
#include <QFile>
#include <QDir>
#include "sbexception.h"
#include "crossplatform.h"
#include <QTextCodec>
#include "utilities.h"

using namespace std;

Filescan::Filescan(Database &datab): db(datab)
{
  localTable = "Lib";
  localTable += db.getUniqueID();
}

Filescan::~Filescan()
{
}

//This method builds a database by scanning the TrackedFolders for media files to add
//Doesn't add files stored in exluded folders
//Returns 1 if successful
int Filescan::build_new()
{
  QStringList TrackedFolders;
  QStringList ExFolders;
  QString homeid;
  try
  {
    db.query("BEGIN;");
    TrackedFolders = db.getFolders(0);
    ExFolders = db.getFolders(1);
    homeid = "local";
    //For every folder to be tracked, run the scanFolder method
    for(int i=0; i<TrackedFolders.size(); i++)
    {
      QDir passpath = QDir(TrackedFolders.at(i));
      scanFolder(passpath, ExFolders, homeid);
    }
    db.updateLocalTimestamp(QString::number(Utilities::getCurrentTimestamp()));
    db.query("COMMIT;");
    qDebug() << "File Scan Completed";
    emit finishedFileScan();
    return 1;
  }
  catch(SBException e)
  {
    throw e;
  }
  return 0;
}

//This method builds a database by scanning the TrackedFolders for media files to add
//Doesn't add files stored in exluded folders
//Returns 1 if successful
//Thsi method differs from the standard build_new because it wipes the current library first.
int Filescan::build_new_clean()
{
  QStringList TrackedFolders;
  QStringList ExFolders;
  QString homeid;
  try
  {
    db.query("BEGIN;");
    QString sql = "DELETE FROM sqlite_sequence WHERE name=\"";
    sql += localTable;
    sql += "\";";
    db.query(sql);
    sql = "DELETE FROM ";
    sql += localTable;
    db.query(sql);
    TrackedFolders = db.getFolders(0);
    ExFolders = db.getFolders(1);
    homeid = "local";
    qDebug() << "Clean File Scan Begun";
    for(int i=0; i<TrackedFolders.size(); i++)
    {
      QDir passpath = QDir(TrackedFolders.at(i));
      scanFolder(passpath, ExFolders, homeid);
    }
    qDebug() << "Clean File Scan Completed";
    db.query("COMMIT;");
    db.updateLocalTimestamp(QString::number(Utilities::getCurrentTimestamp()));
    emit finishedFileScan();
    return 1;
  }
  catch(SBException e)
  {
    throw e;
  }
  return 0;
}

//This method takes a folder path and an array of folders to exclude.
//It calls addFiles on the current folder, then builds a list of directories currently in this folder.
//scanFolder is called on these directories too, so the directory tree is traversed recursively.
//Returns 1 when complete
int Filescan::scanFolder(QDir path, QStringList expaths, QString homeid)
{
  QFileInfoList folderList;
  if(!expaths.isEmpty())
  {
    for(int i=0; i<expaths.size(); i++)
    {
      if(path==expaths[i])
        return 1;
    }
  }
  addFiles(path, homeid);
  path.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
  folderList = path.entryInfoList();

  if(!folderList.isEmpty())
  {
    for(int j=0; j<folderList.size(); j++)
    {

      scanFolder(QDir(folderList.at(j).absoluteFilePath()), expaths, homeid);
    }
  }
  return 1;
}


//This method takes a folder path adds all the media files within it to the database.
//Each file is checked before it is added to see if it is already in the database, if it is then it is skipped.
//If a duplicate file is found, e.g. one matching certain criteria, then it is added to the database with a dup flag set 1
//returns 1 if files are added.
//TODO Deal with album art in some way.
void Filescan::addFiles(QDir path, QString homeID)
{
  QFileInfoList fileList;
  QList<QString> tags;
  path.setFilter(QDir::Files);
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  fileList = path.entryInfoList();
  if(!fileList.isEmpty())
  {

    for(int i = 0; i<fileList.size(); i++)
    {
      if(ismedia(fileList.at(i))==1)
      {
        try
        {
          tags = checktags(file.printMeta(fileList.at(i).absoluteFilePath()), fileList.at(i).fileName());
          db.addFile(fileList.at(i).absoluteFilePath(), fileList.at(i).fileName(), QString::number(fileList.at(i).size()), tags.at(0), tags.at(1), tags.at(2), tags.at(3), tags.at(4), tags.at(5), tags.at(6), tags.at(7), (QString)"1411", fileList.at(i).suffix(), (QString)localTable, homeID);
        }
        catch (SBException e)
        {
            qDebug() << e.getException();
            qDebug() << fileList.at(i).fileName();
            qDebug() << " is broken";
        }
      }
    }
  }
}

void Filescan::addFile(QString filepath, QString homeID)
{
  //QDir* path = new QDir(filepath);
  QList<QString> tags;
  QFileInfo newfile(filepath);
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  if(ismedia(newfile)==1)
  {
    try
    {
      tags = checktags(file.printMeta(newfile.absoluteFilePath()), newfile.fileName());
      db.addFile(newfile.absoluteFilePath(), newfile.fileName(), QString::number(newfile.size()), tags.at(0), tags.at(1), tags.at(2), tags.at(3), tags.at(4), tags.at(5), tags.at(6), tags.at(7), (QString)"1411", newfile.suffix(), (QString)localTable, homeID);
    }
    catch (SBException e)
    {
      qDebug() << e.getException();
      qDebug() << newfile.fileName();
      qDebug() << " is broken";
    }
  }
}



QList<QString> Filescan::checktags(QList<QString> tags, QString filename)
{
  bool ok;
  if(tags.at(0) == "")
    tags.replace(0, "Unknown Artist");
  if(tags.at(1) == "")
    tags.replace(1, "Unknown Album");
  if(tags.at(2) == "")
    tags.replace(2, filename);
  if(tags.at(3) == "")
    tags.replace(3, "Unknown Genre");
  if(tags.at(4).toInt(&ok, 10) == 0)
    tags.replace(4, "0");
  if(tags.at(4) == "")
    tags.replace(4, "0");
  if(tags.at(5) == "")
    tags.replace(5, "-1");
  if(tags.at(6) == "")
    tags.replace(6, "0");
  if(tags.at(7) == "")
    tags.replace(7, "0");
  QString updated0 = tags.at(0);
  QString updated1 = tags.at(1);
  QString updated2 = tags.at(2);
  QString updated3 = tags.at(3);
  tags.replace(0, updated0.replace("\"", "'"));
  tags.replace(1, updated1.replace("\"", "'"));
  tags.replace(2, updated2.replace("\"", "'"));
  tags.replace(3, updated3.replace("\"", "'"));
  QString updated00 = tags.at(0);
  QString updated10 = tags.at(1);
  QString updated20 = tags.at(2);
  QString updated30 = tags.at(3);
  tags.replace(0, updated00.replace(";", ","));
  tags.replace(1, updated10.replace(";", ","));
  tags.replace(2, updated20.replace(";", ","));
  tags.replace(3, updated30.replace(";", ","));
  return tags;
}

//TO DO: Many more compatible filetypes need adding
//This method takes a filepath and returns 1 if the file in question is a media file
int Filescan::ismedia(QFileInfo file)
{
  QString name = file.suffix();
  if( QString::compare("wav",name)==0   || QString::compare("mp3",name)==0   ||
      QString::compare("wma",name)==0   || QString::compare("ogg",name)==0   ||
      QString::compare("aac",name)==0   || QString::compare("mid",name)==0   ||
      QString::compare("m2v",name)==0   || QString::compare("m4v",name)==0   ||
      QString::compare("flac",name)==0  || QString::compare("wmv",name)==0   ||
      QString::compare("mpeg1",name)==0 || QString::compare("oma",name)==0   ||
      QString::compare("mpeg2",name)==0 || QString::compare("ts",name)==0    ||
      QString::compare("divx",name)==0  || QString::compare("vob",name)==0   ||
      QString::compare("dv",name)==0    || QString::compare("avi",name)==0   ||
      QString::compare("flv",name)==0   || QString::compare("mpeg",name)==0  ||
      QString::compare("m1v",name)==0   || QString::compare("mpg",name)==0   ||
      QString::compare("mov",name)==0   || QString::compare("m4a",name)==0   ||
      QString::compare("mp1",name)==0   || QString::compare("mod",name)==0   ||
      QString::compare("m4p",name)==0   || QString::compare("mpeg4",name)==0 ||
      QString::compare("mid",name)==0   )

  {
    return 1;
  }
  return 0;
}

/*//This method takes a filepath and returns 1 if the exact file in question is already in the database
int filescan::isalreadyindat(QDir file)
{
    int i=0;
    if( (db.selectQuery("SELECT Filepath FROM HomeTable WHERE  Filepath = %s", file.filePath())).isNull );
        return 0
    return 1;
}

//this method takes a filepath and returns 1 if this file is a duplicate
int filescan::isdup(QDir file)
{
    QString name = ((file.entryInfoList()).takeFirst()).fileName();
    uint filesize = ((file.entryInfoList()).takeFirst()).size();
    char* meta = libvlc_media_get_meta( libvlc_media_t *p_md, libvlc_meta_t e_meta );
        return 1;
    return 0;
}
 */
