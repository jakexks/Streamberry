#include "database.h"
#include <QtDebug>
#include <QtSql>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QSqlRecord>
#include <QList>
#include "sbexception.h"
#include "crossplatform.h"
#include <utilities.h>


using namespace std;

Database::Database()
{
  db = QSqlDatabase::addDatabase( "QSQLITE" );
  connected = false;
  localUniqueId = "Local";
  dbfilename = "database.sqlite";
  initialise();
}


//deconstructor
Database::~Database()
{
  //if db is connected then it must be closed
  if(connected)
  {
    db.close();
  }
}


void Database::connect(QString &path)
{
  if(!connected)
  {
    db.setDatabaseName(path);

    //if database can't be opened at path, print error and return with error code
    if(!db.open())
    {
      throw SBException(DB, "Database could not be found.");
    }
    else
    {
      connected = true;
    }
  }
}


//create database if one does not exist
void Database::createDatabase(QString &path)
{
  const int sqlstatements = 12;
  //put filename on end of path
  QString filepath = path;
  filepath += dbfilename;
  //sql statements which creates structure. must be split as it doesn't seem to work with just one
  QString sql[sqlstatements];
  sql[0] = "DROP TABLE IF EXISTS \"LibLocal\";";
  sql[1] = "CREATE TABLE \"Lib";
  sql[1] += localUniqueId;
  sql[1] += "\" (\"UniqueID\" VARCHAR DEFAULT \"Local\",  \"Filepath\" VARCHAR NOT NULL PRIMARY KEY UNIQUE , \"Artist\" VARCHAR, \"Album\" VARCHAR, \"Title\" VARCHAR, \"Track\" INTEGER, \"Genre\" VARCHAR, \"Rating\" INTEGER, \"Filename\" VARCHAR NOT NULL , \"Year\" INTEGER, \"Length\" INTEGER NOT NULL, \"Bitrate\" INTEGER, \"Filesize\" INTEGER, \"Timestamp\" INTEGER NOT NULL , \"Filetype\" VARCHAR, \"MusicOrVideo\" INTEGER NOT NULL, \"Deleted\" BOOL NOT NULL DEFAULT 0, \"Hidden\" BOOL NOT NULL DEFAULT 0);";
  sql[2] = "DROP TABLE IF EXISTS \"LibIndex\";";
  sql[3] = "CREATE TABLE \"LibIndex\" (\"ID\" INTEGER PRIMARY KEY  NOT NULL  UNIQUE  check(typeof(\"ID\") = 'integer') , \"Local\" BOOL NOT NULL  DEFAULT 0, \"TimeLastUpdated\" INTEGER NOT NULL , \"TimeLastOnline\" INTEGER NOT NULL , \"UniqueID\" VARCHAR UNIQUE NOT NULL, \"Name\" VARCHAR NOT NULL , \"Online\" BOOL NOT NULL , \"IPAddress\" VARCHAR);";
  sql[4] = "INSERT INTO LibIndex (ID, Local, TimeLastUpdated, TimeLastOnline, UniqueID, Name, Online) VALUES (\"1\", 1, \"";
  sql[4] += QString::number(Utilities::getCurrentTimestamp());
  sql[4] += "\", \"0\", \"Local\", \"Local\", 1);";
  sql[5] = "DROP TABLE IF EXISTS \"Settings\";";
  sql[6] = "CREATE TABLE \"Settings\" (\"Name\" VARCHAR(10) UNIQUE,\"Value\" VARCHAR(20));";
  sql[7] = "DROP TABLE IF EXISTS \"TrackedFolders\";";
  sql[8] = "CREATE TABLE \"TrackedFolders\" (\"Folderpath\" VARCHAR PRIMARY KEY  NOT NULL  UNIQUE);";
  sql[9] = "CREATE TABLE \"ExcludedFolders\" (\"Folderpath\" VARCHAR PRIMARY KEY  NOT NULL  UNIQUE);";
  sql[10] = "CREATE TABLE \"Playlist\" (\"Name\" VARCHAR PRIMARY KEY  NOT NULL  UNIQUE , \"Smart\" BOOL NOT NULL  DEFAULT 0, \"Filter\" VARCHAR, \"Played\" INTEGER NOT NULL)";
  sql[11] = "CREATE TABLE \"PlaylistTracks\" (\"UniqueID\" VARCHAR NOT NULL, \"ID\" VARCHAR NOT NULL,\"Playlist\" VARCHAR NOT NULL )";
  //sql[9] = "DROP TABLE IF EXISTS \"UserTable\";";
  //sql[10] = "CREATE TABLE \"UserTable\" (\"ID\" INTEGER PRIMARY KEY  NOT NULL ,\"Artist\" VARCHAR,\"Album\" VARCHAR,\"Title\" VARCHAR,\"Rating\" INTEGER,\"Filename\" VARCHAR NOT NULL ,\"Year\" DATETIME,\"Length\" INTEGER,\"Bitrate\" INTEGER,\"Filesize\" INTEGER,\"Timestamp\" DATETIME NOT NULL ,\"Filetype\" VARCHAR,\"MusicOrVideo\" INTEGER NOT NULL, \"MusicOrVideo\" INTEGER NOT NULL, \"Deleted\" BOOL DEFAULT 0);";

  //file and directory objects to create file and directories
  QFile dbfile(filepath);
  QDir dir;

  //if the file exists, then you shouldn't be making a new one
  if(dbfile.exists())
  {
    throw SBException(DB, "Database exists but create has been called.");
  }
  try
  {

    //make folder and file
    dir.mkpath(path);
    dbfile.open(QIODevice::ReadWrite);
    dbfile.close();
    //connect to new file as database
    connect(filepath);
    //create database structure by looping through sql statements
    for(int i = 0; i<sqlstatements; i++)
    {
      query(sql[i]);
    }
  }
  catch(SBException e)
  {
    throw e;
  }
}


void Database::initialise()
{
  QString path = CrossPlatform::getAppDataPath();
  QString filepath = path;
  filepath += dbfilename;
  QFile dbfile(filepath);
  //() << path;
  try
  {
    //if the database doesn't exist, create it
    if(!dbfile.exists())
    {
      createDatabase(path);
    }

    connect(filepath);
    setAllOffline();
  }
  catch (SBException e)
  {
    throw e;
  }
}

void Database::startBulk()
{
  try
  {
    //do this because calling begin twice will cause an exception
    try
    {
      query("COMMIT;");
    }
    catch(SBException) {}

    query("BEGIN;");
  }
  catch(SBException e)
  {
    throw e;
  }
}

void Database::endBulk()
{
  try
  {
    //do this because calling COMMIT twice will cause an exception
    try
    {
      query("BEGIN;");
    }
    catch(SBException) {}

    query("COMMIT;");
  }
  catch(SBException e)
  {
    throw e;
  }
}


void Database::setAllOffline()
{
  try
  {
    QString sql = "UPDATE LibIndex SET Online='0' WHERE UniqueID!='Local';";
    query(sql);
  }
  catch(SBException e)
  {
    throw e;
  }
}


QSqlQuery Database::query(QString sql)
{
  if(!connected) throw SBException(DB, "Cannot run query, not connected to database.");
  QSqlQuery query(db);
  query.prepare(sql);
  //qDebug() << sql;
  if(!query.exec())
  {
    QString s = "SQL failed: ";
    s += query.lastError().text();
    s += " | SQL: ";
    s += sql;
    throw SBException(DB, s);
  }
  return query;
}

QSqlQuery Database::querysplit(QString sql)
{
  if(!connected) throw SBException(DB, "Cannot run query, not connected to database.");
  int i=0;
  QList<QString> queries = sql.split(";", QString::SkipEmptyParts);
  QSqlQuery query(db);
  for(i=0; i<queries.size(); i++)
  {
    query.prepare(queries.at(i));
    //if it can't execute, throw exception
    if(!query.exec())
    {
      qDebug() << queries.at(i);
      QString s = "SQL failed: ";
      s += query.lastError().text();
      throw SBException(DB, s);
    }
  }
  return query;
}


void Database::updateLocalTimestamp(QString timestamp)
{
  try
  {
    QString sql = "UPDATE LibIndex SET TimeLastUpdated='";
    sql += timestamp;
    sql += "' WHERE UniqueID='Local'";
    query(sql);
  }
  catch(SBException e)
  {
    throw e;
  }
}


void Database::initialiseScan()
{
  try
  {
    QString sql = "DROP TABLE IF EXISTS \"Lib";
    sql += localUniqueId;
    sql += "Scan\";";
    query(sql);
    sql = "CREATE TABLE \"Lib";
    sql += localUniqueId;
    sql += "Scan\" (\"UniqueID\" VARCHAR DEFAULT \"Local\", \"Filepath\" VARCHAR PRIMARY KEY NOT NULL  UNIQUE , \"Artist\" VARCHAR, \"Album\" VARCHAR, \"Title\" VARCHAR, \"Track\" INTEGER, \"Genre\" VARCHAR, \"Rating\" INTEGER, \"Filename\" VARCHAR NOT NULL , \"Year\" INTEGER, \"Length\" INTEGER NOT NULL, \"Bitrate\" INTEGER, \"Filesize\" INTEGER, \"Timestamp\" INTEGER NOT NULL , \"Filetype\" VARCHAR, \"MusicOrVideo\" INTEGER NOT NULL, \"Deleted\" BOOL NOT NULL DEFAULT 0, \"Hidden\" BOOL NOT NULL DEFAULT 0);";
    query(sql);
    //        sql = "INSERT INTO \"Lib";
    //        sql += localUniqueId;
    //        sql += "Scan\" SELECT * FROM \"Lib";
    //        sql += localUniqueId;
    //        sql += "\" WHERE Deleted='1';";
    //        query(sql);
  }
  catch (SBException e)
  {
    throw e;
  }
}

void Database::completeScan(QString timestamp)
{
  try
  {
    //set all files not found in the scan as deleted
    QString sql = "UPDATE Lib$local SET Deleted='1', Timestamp='$timestamp' WHERE NOT EXISTS (SELECT NULL FROM Lib$localScan WHERE Lib$local.filename=Lib$localScan.filename);";
    sql.replace("$local", localUniqueId);
    sql.replace("$timestamp", timestamp);
    query(sql);

    //make temp tables
    sql = "DROP TABLE IF EXISTS \"LibScanTemp\";";
    //qDebug() << sql;
    query(sql);
    sql = "CREATE TABLE \"LibScanTemp\" (\"UniqueID\" VARCHAR DEFAULT \"Local\", \"Filepath\" VARCHAR PRIMARY KEY NOT NULL  UNIQUE , \"Artist\" VARCHAR, \"Album\" VARCHAR, \"Title\" VARCHAR, \"Track\" INTEGER, \"Genre\" VARCHAR, \"Rating\" INTEGER, \"Filename\" VARCHAR NOT NULL , \"Year\" INTEGER, \"Length\" INTEGER NOT NULL, \"Bitrate\" INTEGER, \"Filesize\" INTEGER, \"Timestamp\" INTEGER NOT NULL , \"Filetype\" VARCHAR, \"MusicOrVideo\" INTEGER NOT NULL, \"Deleted\" BOOL NOT NULL DEFAULT 0, \"Hidden\" BOOL NOT NULL DEFAULT 0);";
    //qDebug() << sql;
    query(sql);

    //copy all records that need updating into a new table
    sql = "INSERT INTO \"LibScanTemp\" SELECT Lib$local.UniqueID, Lib$local.Filepath, Lib$localScan.Artist, Lib$localScan.Album, Lib$localScan.Title, Lib$localScan.Track, Lib$localScan.Genre, Lib$localScan.Rating, Lib$localScan.Filename, Lib$localScan.Year, Lib$localScan.Length, Lib$localScan.Bitrate, Lib$localScan.Filesize, $timestamp, Lib$localScan.Filetype, Lib$localScan.MusicOrVideo, 0 AS Deleted, Lib$local.Hidden FROM Lib$local, Lib$localScan WHERE Lib$local.Filepath=Lib$localScan.Filepath AND Lib$localScan.Filename=Lib$local.Filename AND (Lib$localScan.Artist<>Lib$local.Artist OR Lib$localScan.Album<>Lib$local.Album OR Lib$localScan.Title<>Lib$local.Title OR Lib$localScan.Track<>Lib$local.Track OR Lib$localScan.Genre<>Lib$local.Genre OR Lib$localScan.Rating<>Lib$local.Rating OR Lib$localScan.Year<>Lib$local.Year OR Lib$localScan.Length<>Lib$local.Length OR Lib$localScan.Bitrate<>Lib$local.Bitrate OR Lib$localScan.Filesize<>Lib$local.Filesize OR Lib$localScan.MusicOrVideo<>Lib$local.MusicOrVideo OR Lib$localScan.Deleted<>Lib$local.Deleted);";
    sql.replace("$local", localUniqueId);
    sql.replace("$timestamp", timestamp);
    //qDebug() << sql;
    query(sql);

    //delete the old records that have been updated
    sql = "DELETE FROM Lib$local WHERE EXISTS (SELECT 1 FROM LibScanTemp WHERE Lib$local.Filepath=LibScanTemp.Filepath AND LibScanTemp.Filename=Lib$local.Filename AND (LibScanTemp.Artist<>Lib$local.Artist OR LibScanTemp.Album<>Lib$local.Album OR LibScanTemp.Title<>Lib$local.Title OR LibScanTemp.Track<>Lib$local.Track OR LibScanTemp.Genre<>Lib$local.Genre OR LibScanTemp.Rating<>Lib$local.Rating OR LibScanTemp.Year<>Lib$local.Year OR LibScanTemp.Length<>Lib$local.Length OR LibScanTemp.Bitrate<>Lib$local.Bitrate OR LibScanTemp.Filesize<>Lib$local.Filesize OR LibScanTemp.MusicOrVideo<>Lib$local.MusicOrVideo OR LibScanTemp.Deleted<>Lib$local.Deleted));";
    sql.replace("$local", localUniqueId);
    //qDebug() << sql;
    query(sql);

    //copy back in the update records
    sql = "INSERT INTO Lib$local SELECT * FROM LibScanTemp;";
    sql.replace("$local", localUniqueId);
    // qDebug() << sql;
    query(sql);

    //copy new records into the database
    sql = "INSERT INTO Lib$local SELECT UniqueID, Filepath, Artist, Album, Title, Track, Genre, Rating, Filename, Year, Length, Bitrate, Filesize, Timestamp, Filetype, MusicOrVideo, Deleted, Hidden FROM Lib$localScan WHERE NOT EXISTS (SELECT NULL FROM Lib$local WHERE Lib$local.filename=Lib$localScan.filename);";
    sql.replace("$local", localUniqueId);
    //  qDebug() << sql;
    query(sql);

    //delete temp tables
    sql = "DROP TABLE IF EXISTS \"Lib";
    sql += localUniqueId;
    sql += "Scan\";";
    //  qDebug() << sql;
    query(sql);
    sql = "DROP TABLE IF EXISTS \"LibScanTemp\";";
    //qDebug() << sql;
    query(sql);
  }
  catch (SBException e)
  {
    throw e;
  }
}


void Database::storeSetting(QString name, QString value)
{
  QString sql = "INSERT OR REPLACE INTO Settings (name, value) VALUES (\"";
  sql += name;
  sql += "\", \"";
  sql += value;
  sql += "\");";
  try
  {
    query(sql);
  }
  catch (SBException e)
  {
    throw e;
  }
}


QString Database::getSetting(QString name)
{
  QString sql = "SELECT value FROM Settings WHERE name=\"";
  sql += name;
  sql += "\" LIMIT 1";
  try
  {
    QSqlQuery result = query(sql);

    //if query has returned empty
    if(!result.first())
    {
      return NULL;
    }

    const QSqlRecord r = result.record();
    return r.value("value").toString();
  }
  catch(SBException e)
  {
    throw e;
  }

}

void Database::setNick(QString nick)
{
  QString sql = "UPDATE LibIndex SET Name='";
  sql += nick;
  sql += "' WHERE UniqueID='Local'";
  try
  {
    query(sql);
  }
  catch(SBException e)
  {
    throw e;
  }
}

QString Database::getNick()
{
  QString sql = "SELECT Name FROM LibIndex WHERE UniqueID='Local'";
  try
  {
    QSqlQuery result1 = query(sql);
    result1.first();
    QString result = result1.value(0).toString();
    return result;
  }
  catch(SBException e)
  {
    throw e;
  }
}

void Database::setFolders(QString folders) {
  try
  {
    setFolders(folders.split(";"));
  }
  catch(SBException e)
  {
    throw e;
  }
}


void Database::setFolders(QStringList folderlist)
{
  //NOTE: method implentation assumes low number of folders as it does not detect changes, it just deletes all records before entering. may need to be rewritten to detect changes if large number of folders used.
  QString sql;
  QString folderpath;
  //clear table first
  sql = "DELETE FROM TrackedFolders";

  try
  {
    query(sql);
  }
  catch(SBException e)
  {
    throw e;
  }

  //for every folder in folderlist
  while(!folderlist.isEmpty())
  {
    //removes and returns first folder in list
    folderpath = folderlist.takeFirst();

    //necessary as blank strings can be added otherwise if input doesn't end with semi-colon
    if(folderpath.compare("")!=0)
    {
      sql = "INSERT OR REPLACE INTO TrackedFolders (Folderpath) VALUES (\"";
      sql += folderpath;
      sql += "\");";

      try
      {
        query(sql);
      }
      catch(SBException e)
      {
        throw e;
      }
    }
  }

}


QStringList Database::getFolders(int trackedOrExcluded)
{
  QStringList folderlist;
  QSqlQuery result;
  QString sql;

  if(trackedOrExcluded==0)
  {
    sql = "SELECT * FROM TrackedFolders ORDER BY Folderpath ASC;";
  }
  else
  {
    sql = "SELECT * FROM ExcludedFolders ORDER BY Folderpath ASC;";
  }
  try
  {
    result = query(sql);
    result.first();

    int i = 0;

    //for every record in the query, add it to the folder list as a string
    while(result.isValid())
    {
      //insert result into string list
      folderlist.insert(i, result.value(0).toString());
      i++;
      result.next();
    }
  }
  catch(SBException e)
  {
    throw e;
  }

  return folderlist;
}


QString Database::lastUpdate(QString uniqueid)
{
  QString sql = "SELECT TimeLastUpdated FROM LibIndex WHERE UniqueID='";
  sql += uniqueid;
  sql += "';";

  try
  {
    QSqlQuery result = query(sql);

    //if query has returned empty
    if(!result.first())
    {
      return "";
    }

    const QSqlRecord r = result.record();
    return r.value("TimeLastUpdated").toString();
  }
  catch(SBException e)
  {
    throw e;
  }
}


void Database::setOnline(QString uniqueID, QString status)
{
  QSqlQuery result;
  QString sql;

  sql = "UPDATE LibIndex SET Online='";
  sql += status;
  sql += "' WHERE UniqueID='";
  sql += uniqueID;
  sql += "';";

  try
  {
    result = query(sql);
    emit onlineStatusChange();
  }
  catch(SBException e)
  {
    qDebug() << e.getException();
    throw e;
  }
}


int Database::rowCount(QString tablename)
{
  QSqlQuery result;
  QString sql;

  sql = "SELECT COUNT(*) FROM ";
  sql += tablename;
  sql += ";";

  try
  {
    result = query(sql);
    result.first();
    return result.value(0).toInt();
  }
  catch(SBException e)
  {
    throw e;
  }
}

void Database::addFile(QString filepath, QString filename, QString filesize, QString artist, QString album, QString title, QString genre, QString rating, QString year, QString length, QString trackno, QString bitrate, QString filetype, QString table, QString UniqueID, int mov)
{
  int timestamp = Utilities::getCurrentTimestamp();
  QString musicorvideo;
  if(mov == 2)
    musicorvideo = "1";
  else
    musicorvideo = "0";
  QString sql;
  sql = "INSERT OR REPLACE INTO ";
  sql += table;
  sql += " (UniqueID, Filepath, Artist, Album , Title , Genre, Rating , Filename , Year , Length , Bitrate , Filesize , Timestamp , Filetype, Track, MusicOrVideo) VALUES (\"";
  sql += UniqueID;
  sql += "\", \"";
  sql += filepath;
  sql += "\", \"";
  sql += artist;
  sql += "\", \"";
  sql += album;
  sql += "\", \"";
  sql += title;
  sql += "\", \"";
  sql += genre;
  sql += "\", ";
  sql += rating;
  sql += ", \"";
  sql += filename;
  sql += "\", ";
  sql += year;
  sql += ", ";
  sql += length;
  sql += ", ";
  sql += bitrate;
  sql += ", ";
  sql += filesize;
  sql += ", \"";
  sql += QString::number(timestamp);
  sql += "\", \"";
  sql += filetype;
  sql += "\", ";
  sql += trackno;
  sql += ", \"";
  sql += musicorvideo;
  sql += "\");";
  //qDebug() << sql;
  try
  {
    query(sql);
  }
  catch(SBException e)
  {
    throw e;
  }
}


int Database::deleteFile(QString id, QString table)
{
  QSqlQuery result;
  QString sql = "UPDATE Lib";
  sql += table;
  sql += " SET Deleted = 1 WHERE Filepath=\"";
  sql += id;
  sql +="\"";

  try
  {
    result = query(sql);
    return result.numRowsAffected();
  }
  catch(SBException e)
  {
    throw e;
  }
}


QList<QSqlRecord>* Database::searchDb(int type, QString playlist, QString searchtxt, QList<QString>& sortcols, QList<QString> order, int musicorvideo)
{
  QString condition(" WHERE 1=1");
  QString ordering;
  QString sql;
  QSqlQuery result;
  QList<QSqlRecord> users;
  QList<QSqlRecord> *files;
  searchtxt = searchtxt.trimmed();
  QStringList searches = searchtxt.split(" ", QString::SkipEmptyParts);

  //work out condition
  switch(type)
  {
  case 1:
    foreach(QString str, searches)
    {
      condition += " AND (Artist LIKE \"%";
      condition += str;
      condition += "%\")";
    }
    break;
    case 2:
    foreach(QString str, searches)
    {
      condition += " AND (Title LIKE \"%";
      condition += str;
      condition += "%\")";
    }
    break;
    case 3:
    foreach(QString str, searches)
    {
      condition += " AND (Genre LIKE \"%";
      condition += str;
      condition += "%\")";
    }
    case 4:
    foreach(QString str, searches)
    {
      condition += " AND (Album LIKE \"%";
      condition += str;
      condition += "%\")";
    }
    case 5:
    foreach(QString str, searches)
    {
      condition += " AND (Length LIKE \"%";
      condition += str;
      condition += "%\")";
    }
    default:
    foreach(QString str, searches)
    {
      condition += " AND ((Artist LIKE \"%";
      condition += str;
      condition += "%\") OR (Title LIKE \"%";
      condition += str;
      condition += "%\") OR (Album LIKE \"%";
      condition += str;
      condition += "%\") OR (Genre LIKE \"%";
      condition += str;
      condition += "%\"))";
    }
    break;
  }

  condition += " AND (Deleted='0')";

  switch(musicorvideo)
  {
  case 0:
  case 1:
    condition += " AND MusicOrVideo='";
    condition += QString::number(musicorvideo);
    condition += "'";
  default:
    break;
  }

  int sortcount = sortcols.length();

  if(sortcount>0)
  {
    ordering += " ORDER BY ";
    ordering += sortcols.at(0);
    ordering += " ";
    ordering += order.at(0);

    for(int i = 1; i < sortcount; i++)
    {
      ordering += ", ";
      ordering += sortcols.at(i);
      ordering += " ";
      ordering += order.at(i);
    }

    ordering += ", Track ASC";
  }

  try
  {
    //select all unique ids from libindex
    sql = "SELECT UniqueID FROM LibIndex WHERE (Online=1)";
    result = query(sql);
    result.first();

    //for every table found add it to the users list
    while(result.isValid())
    {
      users.append(result.record());
      result.next();
    }

    //get local library and add to files list
    //sql = "SELECT * FROM LibLocal";
    //sql += condition;
    //result = query(sql);
    //result.first();
    files = new QList<QSqlRecord>();
    // while(result.isValid())
    //{
    //files->append(result.record());
    //result.next();
    //}

    //for every user
    while(!users.isEmpty())
    {
      //get library and add to files list
      sql = "SELECT * FROM Lib";
      QString id = users.takeFirst().value(0).toString();
      sql += id;

      if(playlist!="")
      {
        sql += ", PlaylistTracks";
      }

      sql += condition;

      if(playlist!="")
      {
        sql += " AND Playlist='";
        sql += playlist;
        sql += "' AND PlaylistTracks.ID=Filepath AND PlaylistTracks.UniqueID='";
        sql += id;
        sql += "'";
      }
      //qDebug() << sql;
      sql += ordering;
      result = query(sql);

      result.first();
      while(result.isValid())
      {
        QSqlRecord copy(result.record());

        QSqlField temp("Length", QVariant::String);
        QString intval = result.record().field("Length").value().toString();
        QString timeval = Utilities::intToTime(intval.toInt());
        temp.setValue( timeval );
        int pos = copy.indexOf("Length");
        copy.remove(pos);
        copy.insert(pos, temp);

        if(result.record().field("Filepath").value().toString().indexOf("\\;") != -1)
        {
          const QString fieldname = "Filepath";
          QSqlField temp(fieldname, QVariant::String);
          temp.setValue(result.record().field(fieldname).value().toString().replace("\\;",";"));
          int pos = copy.indexOf(fieldname);
          copy.remove(pos);
          copy.insert(pos, temp);
        }
        if(result.record().field("Artist").value().toString().indexOf("\\;") != -1)
        {
          const QString fieldname = "Artist";
          QSqlField temp(fieldname, QVariant::String);
          temp.setValue(result.record().field(fieldname).value().toString().replace("\\;",";"));
          int pos = copy.indexOf(fieldname);
          copy.remove(pos);
          copy.insert(pos, temp);
        }
        if(result.record().field("Album").value().toString().indexOf("\\;") != -1)
        {
          const QString fieldname = "Album";
          QSqlField temp(fieldname, QVariant::String);
          temp.setValue(result.record().field(fieldname).value().toString().replace("\\;",";"));
          int pos = copy.indexOf(fieldname);
          copy.remove(pos);
          copy.insert(pos, temp);
        }

        files->append(copy);

        result.next();
      }
    }
  }
  catch(SBException e)
  {
    throw e;
  }


  return files;

}


QString Database::getUniqueID()
{
  QString sql = "SELECT UniqueID FROM LibIndex WHERE Local = 1";
  try
  {
    QSqlQuery result1 = query(sql);
    result1.first();
    QString result = result1.value(0).toString();
    return result;
  }
  catch(SBException e)
  {
    throw e;
  }
}


void Database::setIPaddress(QString uniqueID, QString ipaddress)
{
  QString sql;

  sql = "UPDATE LibIndex SET IPAddress='";
  sql += ipaddress;
  sql += "' WHERE UniqueID='";
  sql += uniqueID;
  sql += "';";

  try
  {
    query(sql);
  }
  catch(SBException e)
  {
    qDebug() << e.getException();
    throw e;
  }
}


QString Database::getIPfromUID(QString uniqueID)
{
  QSqlQuery result;
  QString sql;

  sql = "SELECT IPAddress FROM LibIndex WHERE UniqueID='";
  sql += uniqueID;
  sql += "' LIMIT 1;";

  try
  {
    result = query(sql);
    if(!result.first())
    {
      return NULL;
    }
    const QSqlRecord r = result.record();
    return r.value("IPAddress").toString();
  }
  catch(SBException e)
  {
    qDebug() << e.getException();
    throw e;
  }
}

QString Database::changesSinceTime(int timestamp, QString uniqueID)
{
  QString sql;
  QString final;
  QSqlQuery result;

  sql = "SELECT * FROM Lib";
  sql += localUniqueId;
  sql += " WHERE (Timestamp >";
  sql += QString::number(timestamp);
  sql += ");";

  try
  {
    result = query(sql);
    result.first();

    final = "CREATE TABLE IF NOT EXISTS \"Lib";
    final += uniqueID;
    final += "\" (\"UniqueID\" VARCHAR DEFAULT \"";
    final += uniqueID;
    final += "\", \"Filepath\" VARCHAR PRIMARY KEY NOT NULL, \"Artist\" VARCHAR, \"Album\" VARCHAR, \"Title\" VARCHAR, \"Genre\" VARCHAR, \"Rating\" INTEGER, \"Filename\" VARCHAR NOT NULL , \"Year\" INTEGER, \"Track\" INTEGER, \"Length\" INTEGER NOT NULL, \"Bitrate\" INTEGER, \"Filesize\" INTEGER, \"Timestamp\" INTEGER NOT NULL , \"Filetype\" VARCHAR, \"MusicOrVideo\" INTEGER NOT NULL, \"Deleted\" BOOL NOT NULL DEFAULT 0); \x1D";

    while(result.isValid())
    {
      if(result.record().value("Deleted").toInt()==1)
      {
        final += "DELETE FROM Lib";
        final += uniqueID;
        final += " WHERE Filepath='";
        final += result.record().value("Filepath").toString();
        final += "'; \x1D";
      } else {
        final += "INSERT OR REPLACE INTO Lib";
        final += uniqueID;
        final += " (Filepath, Artist, Album , Title , Genre, Rating , Filename , Year , Track, Length , Bitrate , Filesize , Timestamp , Filetype, MusicOrVideo) VALUES (\"";
        final += result.record().value("Filepath").toString();
        final += "\", \"";
        final += result.record().value("Artist").toString();
        final += "\", \"";
        final += result.record().value("Album").toString();
        final += "\", \"";
        final += result.record().value("Title").toString();
        final += "\", \"";
        final += result.record().value("Genre").toString();
        final += "\", \"";
        final += result.record().value("Rating").toString();
        final += "\", \"";
        final += result.record().value("Filename").toString();
        final += "\", \"";
        final += result.record().value("Year").toString();
        final += "\", \"";
        final += result.record().value("Track").toString();
        final += "\", \"";
        final += result.record().value("Length").toString();
        final += "\", \"";
        final += result.record().value("Bitrate").toString();
        final += "\", \"";
        final += result.record().value("Filesize").toString();
        final += "\", \"";
        final += result.record().value("Timestamp").toString();
        final += "\", \"";
        final += result.record().value("Filetype").toString();
        final += "\", \"";
        final += result.record().value("MusicOrVideo").toString();
        //group separator
        final += "\"); \x1D";
      }
      result.next();
    }

    final += "UPDATE LibIndex SET TimeLastUpdated='";
    final += lastUpdate("Local");
    final += "' WHERE UniqueID='";
    final += uniqueID;
    final += "';";
  }
  catch(SBException e)
  {
    throw e;
  }

  return final;
}


void Database::makeUser(QString timeLastUpdated, QString timeLastOnline, QString uniqueID, QString name)
{
  try
  {
    QSqlQuery result;
    QString sql = "SELECT COUNT(*) FROM LibIndex WHERE UniqueID='";
    sql += uniqueID;
    sql += "';";

    result = query(sql);
    result.first();
    if(result.isValid())
    {
      if(result.value(0).toInt()<=0)
      {
        sql = "INSERT INTO LibIndex (Local, TimeLastUpdated, TimeLastOnline, UniqueID, Name, Online) VALUES ('0', '";
        sql += timeLastUpdated;
        sql += "', '";
        sql += timeLastOnline;
        sql += "', '";
        sql += uniqueID;
        sql += "', '";
        sql += name;
        sql += "', '0');";

        result = query(sql);
      }
    }
  }
  catch(SBException e)
  {
    throw e;
  }
}

QSqlQuery Database::GetPlaylistInfo(QString playlistName)
{
  QString sql = "SELECT * FROM Playlist WHERE Name =\"";
  sql += playlistName;
  sql += "\";";

  QSqlQuery result = query(sql);
  return result;
}


QSqlQuery Database::GetPlaylistTracks(QString playlistName)
{
  QSqlQuery result;
  QString sql = "SELECT * FROM PlaylistTracks WHERE Playlist =\"";
  sql += playlistName;
  sql += "\";";
  result = query(sql);
  return result;
}


void Database::PlaylistSave(QString name, int smart, QString filter)
{
  QString test;
  test.setNum(Utilities::getCurrentTimestamp());
  QString value;
  if(smart == 1)
    value = "1";
  else
    value = "0";
  QString sql = "INSERT OR REPLACE INTO Playlist (Name, Smart, Filter, Played) VALUES (\"";
  sql += name;
  sql += "\",";
  sql += value;
  sql += ",\"";
  sql += filter;
  sql += "\",";
  sql += test;
  sql += ");";
  query(sql);
}


void Database::PlaylistAddTracks(QList<QString> Filepaths, QList<QString> UniqueIDs, QString Playlist)
{
  int i=0;
  QString sql = "DELETE FROM PlaylistTracks WHERE Playlist = \"";
  sql += Playlist;
  sql += "\";";
  query(sql);
  for(i = 0; i < Filepaths.size() ; i++)
  {
    QString path = Filepaths.at(i);
    QString uniqueid = UniqueIDs.at(i);
    QString sql;
    sql += "INSERT INTO PlaylistTracks (UniqueID, ID, Playlist) VALUES (\"";
    sql += uniqueid;
    sql += "\", \"";
    sql += path;
    sql += "\", \"";
    sql += Playlist;
    sql += "\");";

    query(sql);
  }
}


QList<QSqlRecord>* Database::getTracks( QList<QString> Filepaths, QList<QString> UniqueIDs)
{
  QString trackid;
  QList<QSqlRecord>* result = new QList<QSqlRecord>();
  int i =0;
  for(i = 0; i < (Filepaths.size()) ; i++)
  {
    QString path = Filepaths.at(i);
    QString id = UniqueIDs.at(i);
    QString sql = "SELECT * FROM ";
    sql += "Lib";
    sql += id;
    sql += " WHERE UniqueID = \"";
    sql += id;
    sql += "\" AND Filepath = \"";
    sql += path;
    sql += "\";";

    QSqlQuery queryresult = query(sql);
    queryresult.first();
    result->append(queryresult.record());
  }
  return result;
}


void Database::removePlaylist(QString name)
{
  QString sql = "DELETE FROM Playlist WHERE Name =\"";
  sql += name;
  sql += "\";";
  sql += "DELETE FROM PlaylistTracks WHERE Playlist = \"";
  sql += name;
  sql += "\";";
  query(sql);
}


QList<QSqlRecord>* Database::getAllPlaylists()
{
  QList<QSqlRecord>* result = new QList<QSqlRecord>;
  QString sql = "SELECT * FROM Playlist ORDER BY Played DESC";
  //QString sql = "SELECT * FROM Playlist";
  QSqlQuery queryresult = query(sql);
  while(queryresult.next())
    result->append(queryresult.record());
  return result;
}

void Database::togglehidden(QString file, QString uniqueID)
{
  QString sql = "SELECT Hidden FROM Lib";
  QString h;
  sql += uniqueID;
  sql += " WHERE Filepath = \"";
  sql +=file;
  sql +="\";";

  QSqlQuery queryresult = query(sql);
  queryresult.first();
  const QSqlRecord r = queryresult.record();
  if( r.value("Hidden").toString() == "0")
    h = "1";
  else
    h = "0";
  QString sql1 = "UPDATE Lib";
  sql1 += uniqueID;
  sql1 += " SET Hidden=";
  sql1 += h;
  sql1 += " WHERE Filepath = \"";
  sql1 +=file;
  sql1 +="\";";
  //qDebug() << sql1;
  query(sql1);
  return;
}
