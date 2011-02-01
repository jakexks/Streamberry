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
#include "utilities.h"


using namespace std;

Database::Database()
{
    db = QSqlDatabase::addDatabase( "QSQLITE" );
    connected = false;
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
    //put filename on end of path
    QString filepath = path;
    filepath += dbfilename;
    //sql statements which creates structure. must be split as it doesn't seem to work with just one
    QString sql[10];
    sql[0] = "DROP TABLE IF EXISTS \"HomeTable\";";
    sql[1] = "CREATE TABLE \"LibLocal\" (\"ID\" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , \"Filepath\" VARCHAR NOT NULL  UNIQUE , \"Artist\" VARCHAR, \"Album\" VARCHAR, \"Title\" VARCHAR, \"Genre\" VARCHAR, \"Rating\" INTEGER, \"Filename\" VARCHAR NOT NULL , \"Year\" INTEGER, \"Length\" INTEGER NOT NULL, \"Bitrate\" INTEGER, \"Filesize\" INTEGER, \"Timestamp\" INTEGER NOT NULL , \"Filetype\" VARCHAR, \"Deleted\" BOOL NOT NULL DEFAULT 0);";
    sql[2] = "DROP TABLE IF EXISTS \"LibIndex\";";
    sql[3] = "CREATE TABLE \"LibIndex\" (\"ID\" INTEGER PRIMARY KEY  NOT NULL  UNIQUE  check(typeof(\"ID\") = 'integer') , \"Local\" BOOL NOT NULL  DEFAULT 0, \"TimeLastUpdated\" INTEGER NOT NULL , \"TimeLastOnline\" INTEGER NOT NULL , \"UniqueID\" VARCHAR UNIQUE NOT NULL, \"Name\" VARCHAR NOT NULL , \"Online\" BOOL NOT NULL );";
    sql[4] = "INSERT INTO LibIndex (ID, Local, TimeLastUpdated, TimeLastOnline, UniqueID, Name, Online) VALUES (\"1\", 1, \"";
    sql[4] += QString::number(Utilities::getCurrentTimestamp());
    sql[4] += "\", \"0\", \"-1\", \"Local\", 1);";
    sql[5] = "DROP TABLE IF EXISTS \"Settings\";";
    sql[6] = "CREATE TABLE \"Settings\" (\"Name\" VARCHAR(10) UNIQUE,\"Value\" INTEGER);";
    sql[7] = "DROP TABLE IF EXISTS \"TrackedFolders\";";
    sql[8] = "CREATE TABLE \"TrackedFolders\" (\"Folderpath\" VARCHAR PRIMARY KEY  NOT NULL  UNIQUE);";
    sql[9] = "CREATE TABLE \"ExcludedFolders\" (\"Folderpath\" VARCHAR PRIMARY KEY  NOT NULL  UNIQUE);";
    //sql[9] = "DROP TABLE IF EXISTS \"UserTable\";";
    //sql[10] = "CREATE TABLE \"UserTable\" (\"ID\" INTEGER PRIMARY KEY  NOT NULL ,\"Artist\" VARCHAR,\"Album\" VARCHAR,\"Title\" VARCHAR,\"Rating\" INTEGER,\"Filename\" VARCHAR NOT NULL ,\"Year\" DATETIME,\"Length\" INTEGER,\"Bitrate\" INTEGER,\"Filesize\" INTEGER,\"Timestamp\" DATETIME NOT NULL ,\"Filetype\" VARCHAR,\"Deleted\" BOOL DEFAULT 0);";

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
        for(int i = 0; i<10; i++)
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

    try
    {
        //if the database doesn't exist, create it
        if(!dbfile.exists())
        {
            createDatabase(path);
        }

        connect(filepath);
    }
    catch (SBException e)
    {
        throw e;
    }
}

QSqlQuery Database::query(QString sql)
{
    if(!connected) throw SBException(DB, "Cannot run query, not connected to database.");
    QSqlQuery query(db);
    query.prepare(sql);

    //if it can't execute, throw exception
    if(!query.exec())
    {
        QString s = "SQL failed: ";
        s += query.lastError().text();
        throw SBException(DB, s);
    }
    return query;
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

void Database::addFile(QString filepath, QString filename, QString filesize, QString artist, QString album, QString title, QString genre, QString rating, QString year, QString length, QString bitrate, QString filetype, QString table)
{
    int timestamp = Utilities::getCurrentTimestamp();
    QString sql;

    sql = "INSERT OR REPLACE INTO ";
    sql += table;
    sql += " (Filepath, Artist, Album , Title , Genre, Rating , Filename , Year , Length , Bitrate , Filesize , Timestamp , Filetype) VALUES (\"";
    sql += filepath;
    sql += "\", \"";
    sql += artist;
    sql += "\", \"";
    sql += album;
    sql += "\", \"";
    sql += title;
    sql += "\", \"";
    sql += genre;
    sql += "\", \"";
    sql += rating;
    sql += "\", \"";
    sql += filename;
    sql += "\", \"";
    sql += year;
    sql += "\", \"";
    sql += length;
    sql += "\", \"";
    sql += bitrate;
    sql += "\", \"";
    sql += filesize;
    sql += "\", \"";
    sql += QString::number(timestamp);
    sql += "\", \"";
    sql += filetype;
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

int Database::deleteFile(QString id, QString table)
{
    QSqlQuery result;
    QString sql = "DELETE FROM ";
    sql += table;
    sql += " WHERE id=\"";
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

QList<QSqlRecord> Database::searchDb(int type, QString searchtxt)
{
    QString condition;
    QString sql;
    QSqlQuery result;
    QList<QSqlRecord> users;
    QList<QSqlRecord> files;

    //work out condition
    switch(type)
    {
    case 1:
        condition = "WHERE (Artist LIKE \"%";
        condition += searchtxt;
        condition += "%\")";
        break;
    case 2:
        condition = "WHERE (Title LIKE \"%";
        condition += searchtxt;
        condition += "%\")";
        break;
    case 3:
        condition = "WHERE (Genre LIKE \"%";
        condition += searchtxt;
        condition += "%\")";
    default:
        condition = "WHERE (Artist LIKE \"%";
        condition += searchtxt;
        condition += "%\") OR (Title LIKE \"%";
        condition += searchtxt;
        condition += "%\") OR (Genre LIKE \"%";
        condition += searchtxt;
        condition += "%\")";
        break;
    }

    try
    {
        //select all unique ids from libindex
        sql = "SELECT UniqueID FROM LibIndex WHERE (UniqueID!=-1) AND (Online=1)";

        result = query(sql);
        result.first();

        //for every table found add it to the users list
        while(result.isValid())
        {
            users.append(result.record());
            result.next();
        }

        //get local library and add to files list
        sql = "SELECT * FROM LibLocal";
        result = query(sql);
        result.first();
        while(result.isValid())
        {
            files.append(result.record());
            result.next();
        }

        //for every user
        while(!users.isEmpty())
        {
            //get library and add to files list
            sql = "SELECT * FROM Lib";
            sql += users.takeFirst().value(0).toString();

            result = query(sql);
            result.first();
            while(result.isValid())
            {
                files.append(result.record());
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

QString Database::changesSinceTime(int timestamp, QString uniqueID)
{
    QString sql;
    QString final;
    QSqlQuery result;

    sql = "SELECT * FROM LibLocal WHERE (Timestamp >";
    sql += QString::number(timestamp);
    sql += ");";

    try
    {
        result = query(sql);
        result.first();

        final = "CREATE TABLE IF NOT EXISTS \"Lib";
        final += uniqueID;
        final += "\" (\"ID\" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , \"Filepath\" VARCHAR NOT NULL  UNIQUE , \"Artist\" VARCHAR, \"Album\" VARCHAR, \"Title\" VARCHAR, \"Genre\" VARCHAR, \"Rating\" INTEGER, \"Filename\" VARCHAR NOT NULL , \"Year\" INTEGER, \"Length\" INTEGER NOT NULL, \"Bitrate\" INTEGER, \"Filesize\" INTEGER, \"Timestamp\" INTEGER NOT NULL , \"Filetype\" VARCHAR, \"Deleted\" BOOL NOT NULL DEFAULT 0); \x1D";

        while(result.isValid())
        {
            final += "INSERT OR REPLACE INTO Lib";
            final += uniqueID;
            final += " (Filepath, Artist, Album , Title , Genre, Rating , Filename , Year , Length , Bitrate , Filesize , Timestamp , Filetype) VALUES (\"";
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
            final += result.record().value("Length").toString();
            final += "\", \"";
            final += result.record().value("Bitrate").toString();
            final += "\", \"";
            final += result.record().value("Filesize").toString();
            final += "\", \"";
            final += result.record().value("Timestamp").toString();
            final += "\", \"";
            final += result.record().value("Filetype").toString();
            //group separator
            final += "\"); \x1D";
            result.next();
        }

    }
    catch(SBException e)
    {
        throw e;
    }

    return final;

}
