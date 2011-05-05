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
#define readwma 1

using namespace std;

#define BUFFERSIZE 1500

Filescan::Filescan(Database &datab): db(datab)
{
  localTable = "Lib";
  localTable += db.getUniqueID();
  localTableScan = localTable;
  localTableScan += "Scan";
  filestoadd = NULL;
  scanning = false;
}

Filescan::~Filescan()
{
  if(filestoadd!=NULL)
  {
    delete filestoadd;
  }
}

//This method builds a database by scanning the TrackedFolders for media files to add
//Doesn't add files stored in exluded folders
//Returns 1 if successful
void Filescan::scan()
{
  //just to make sure the scanner never runs more than once
  if(!scanning)
  {
    QStringList TrackedFolders;
    QStringList ExFolders;
    QString homeid;
    try
    {
      qDebug() << "Filescan Started.";
      QTime scantime;
      scantime.start();

      scanning = true;
      TrackedFolders = db.getFolders(0);
      ExFolders = db.getFolders(1);
      homeid = "Local";

      //setup temporary table in database
      db.initialiseScan();

      if(filestoadd!=NULL)
      {
        delete filestoadd;
      }

      filestoadd = new QList<QStringList>();

      //For every folder to be tracked, run the scanFolder method
      for(int i=0; i<TrackedFolders.size(); i++)
      {
        QDir passpath = QDir(TrackedFolders.at(i));
        scanFolder(passpath, ExFolders, homeid);
      }

      //if buffer has stuff in it, get rid of it
      if(filestoadd!=NULL)
      {
        int size = filestoadd->size();
        db.startBulk();

        for(int i = 0; i<size; i++)
        {
          try
          {
            db.addFile(filestoadd->at(i).at(0), filestoadd->at(i).at(1), filestoadd->at(i).at(2), filestoadd->at(i).at(3), filestoadd->at(i).at(4), filestoadd->at(i).at(5), filestoadd->at(i).at(6), filestoadd->at(i).at(7), filestoadd->at(i).at(8), filestoadd->at(i).at(9), filestoadd->at(i).at(10), filestoadd->at(i).at(11), filestoadd->at(i).at(12), filestoadd->at(i).at(13), filestoadd->at(i).at(14), filestoadd->at(i).at(15).toInt());
          }
          catch(SBException e)
          {
            qDebug() << e.getException();
          }
        }

        db.endBulk();
        delete filestoadd;
        filestoadd = NULL;
      }

      db.completeScan(QString::number(Utilities::getCurrentTimestamp()));
      db.updateLocalTimestamp(QString::number(Utilities::getCurrentTimestamp()));

      qDebug() << "File Scan Complete.\nScan Time Elapsed: " << scantime.elapsed() << "ms";
    }
    catch(SBException e)
    {
      scanning = false;
      //just incase it crashed on db
      db.endBulk();
      qDebug() << e.getException();
      //throw e;
    }

    emit finishedFileScan();
  }

  scanning = false;
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

      QFileInfo newfile = fileList.at(i);
      int mov =0;
      mov = ismedia(newfile);
      if(mov!=0)
      {
        try
        {
          tags = checktags(file.printMeta(newfile.absoluteFilePath()), newfile.fileName());
          QString filepathnew = newfile.absoluteFilePath();
          filepathnew.replace(";", "\\;");
          QStringList file;
          file << filepathnew << newfile.fileName() << QString::number(newfile.size()) << tags.at(0) << tags.at(1) << tags.at(2) << tags.at(3) << tags.at(4) << tags.at(5) << tags.at(6) << tags.at(7) << (QString)"1411" << newfile.suffix() << localTableScan << homeID << QString::number(mov);
          filestoadd->append(file);
          //db.addFile(filepathnew, newfile.fileName(), QString::number(newfile.size()), tags.at(0), tags.at(1), tags.at(2), tags.at(3), tags.at(4), tags.at(5), tags.at(6), tags.at(7), (QString)"1411", newfile.suffix(), (QString)localTable, homeID, mov);
        }
        catch (SBException e)
        {
          qDebug() << e.getException();
          qDebug() << newfile.fileName();
          qDebug() << " is broken";
        }
      }
      //            else if(mov==2)
      //            {
      //                try
      //                {
      //                    tags = checktags(file.printMeta(newfile.absoluteFilePath()), newfile.fileName());
      //                    QString filepathnew = newfile.absoluteFilePath();
      //                    filepathnew.replace(";", "\\;");
      //                    //qDebug() << "HERE Now";
      //                   // qDebug() << mov;
      //                    QStringList file;
      //                    file << filepathnew << newfile.fileName() << QString::number(newfile.size()) << tags.at(0) << tags.at(1) << tags.at(2) << tags.at(3) << tags.at(4) << tags.at(5) << tags.at(6) << tags.at(7) << (QString)"1411" << newfile.suffix() << localTableScan << homeID << QString::number(mov);
      //                    filestoadd->append(file);
      //                    //db.addFile(filepathnew, newfile.fileName(), QString::number(newfile.size()), "Unknown Director", "Unknown Producer", newfile.fileName(), "Unknown Genre", "0", "-1", "0", "0", (QString)"1411", newfile.suffix(), localTable, homeID, mov);
      //                }
      //                catch (SBException e)
      //                {
      //                    qDebug() << e.getException();
      //                    qDebug() << newfile.fileName();
      //                    qDebug() << " is broken";
      //                }
      //            }
    }
  }

  //if there are 'buffer size' or more files to add, add them all
  if(filestoadd!=NULL && filestoadd->size()>=BUFFERSIZE)
  {
    int size = filestoadd->size();
    db.startBulk();

    for(int i = 0; i<size; i++)
    {
      if(i%100 == 0)
        qDebug() << "Still filescanning...";
      try
      {
        qDebug() << filestoadd->at(i).at(9);
        db.addFile(filestoadd->at(i).at(0), filestoadd->at(i).at(1), filestoadd->at(i).at(2), filestoadd->at(i).at(3), filestoadd->at(i).at(4), filestoadd->at(i).at(5), filestoadd->at(i).at(6), filestoadd->at(i).at(7), filestoadd->at(i).at(8), filestoadd->at(i).at(9), filestoadd->at(i).at(10), filestoadd->at(i).at(11), filestoadd->at(i).at(12), filestoadd->at(i).at(13), filestoadd->at(i).at(14), filestoadd->at(i).at(15).toInt());
      }
      catch(SBException e)
      {
        qDebug() << e.getException();
      }
    }

    db.endBulk();
    qDebug() << "Still filescanning...";

    delete filestoadd;
    filestoadd = new QList<QStringList>();
  }
}



void Filescan::addFile(QString filepath, QString homeID)
{


  /*
  //QDir* path = new QDir(filepath);
  QList<QString> tags;
  QFileInfo newfile(filepath);
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  int mov;
  mov = ismedia(newfile);
  if(mov ==1 )
  {
    try
    {
      tags = checktags(file.printMeta(newfile.absoluteFilePath()), newfile.fileName());
      QString filepathnew = newfile.absoluteFilePath();
      filepathnew.replace(";", "\\;");
      //         filepath      filename                       filesize              artist     album         title    genre      rating          year         length    trackno    bitrate         filetype          table       UniqueID   mov
      db.addFile(filepathnew, newfile.fileName(), QString::number(newfile.size()), tags.at(0), tags.at(1), tags.at(2), tags.at(3), tags.at(4), tags.at(5), tags.at(6), tags.at(7), (QString)"1411", newfile.suffix(), localTableScan, homeID, mov);
    }
    catch (SBException e)
    {
      qDebug() << e.getException();
      qDebug() << newfile.fileName();
      qDebug() << " is broken";
    }
  }
  else if(mov==2)
  {
    try
    {
      QString filepathnew = newfile.absoluteFilePath();
      filepathnew.replace(";", "\\;");
      db.addFile(filepathnew, newfile.fileName(), QString::number(newfile.size()), "Unknown Director", "Unknown Producer", newfile.fileName(), "Unknown Genre", "0", "-1", "0", "0", (QString)"1411", newfile.suffix(), (QString)localTable, homeID, mov);
    }
    catch (SBException e)
    {
      qDebug() << e.getException();
      qDebug() << newfile.fileName();
      qDebug() << " is broken";
    }
  }
  */
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
  tags.replace(0, updated00.replace(";", "\\;"));
  tags.replace(1, updated10.replace(";", "\\;"));
  tags.replace(2, updated20.replace(";", "\\;,"));
  tags.replace(3, updated30.replace(";", "\\;"));
  return tags;
}

//TO DO: Many more compatible filetypes need adding
//This method takes a filepath and returns 1 if the file in question is an audio file, 2 if it is a video file and 0 otherwise
int Filescan::ismedia(QFileInfo file)
{
  QString name = file.suffix();
  if( (QString::compare("wma",name)==0) && readwma == 0  )
    return 0;
#ifdef Q_WS_MAC
  if( QString::compare("wma",name)==0  )
    return 0;
#endif
  if( QString::compare("wav",name)==0   || QString::compare("mp3",name)==0   ||
      QString::compare("wma",name)==0   || QString::compare("ogg",name)==0   ||
      QString::compare("aac",name)==0   || QString::compare("mid",name)==0   ||
      QString::compare("flac",name)==0  || QString::compare("oma",name)==0   ||
      QString::compare("m4a",name)==0   || QString::compare("mp1",name)==0   ||
      QString::compare("mod",name)==0   || QString::compare("m4p",name)==0   ||
      QString::compare("mid",name)==0   )
  {
    return 1;
  }
  else if(
      QString::compare("m2v",name)==0   || QString::compare("m4v",name)==0   ||
      QString::compare("wmv",name)==0   || QString::compare("mpeg4",name)==0 ||
      QString::compare("mpeg1",name)==0 || QString::compare("mpeg2",name)==0 ||
      QString::compare("ts",name)==0    || QString::compare("divx",name)==0  ||
      QString::compare("vob",name)==0   || QString::compare("dv",name)==0    ||
      QString::compare("avi",name)==0   || QString::compare("flv",name)==0   ||
      QString::compare("mpeg",name)==0  || QString::compare("m1v",name)==0   ||
      QString::compare("mpg",name)==0   || QString::compare("mov",name)==0
      )
  {
    return 2;
  }
  else
    return 0;
}

bool Filescan::currentlyScanning()
{
  return scanning;
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
