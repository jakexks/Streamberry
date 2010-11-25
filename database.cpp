#include "database.h"
#include <QtDebug>
#include <QtSql>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QDir>
#include "sbexception.h"
#include "crossplatform.h"
#include "iostream"

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
    sql[1] = "CREATE TABLE \"HomeTable\" (\"ID\" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , \"Filepath\" VARCHAR NOT NULL  UNIQUE , \"Artist\" VARCHAR, \"Album\" VARCHAR, \"Title\" VARCHAR, \"Rating\" INTEGER, \"Filename\" VARCHAR NOT NULL , \"Year\" DATETIME, \"Length\" INTEGER, \"Bitrate\" INTEGER, \"Filesize\" INTEGER, \"Timestamp\" DATETIME NOT NULL , \"Filetype\" VARCHAR);";
    sql[2] = "DROP TABLE IF EXISTS \"LibIndex\";";
    sql[3] = "CREATE TABLE \"LibIndex\" (\"ID\" INTEGER PRIMARY KEY  NOT NULL  UNIQUE  check(typeof(\"ID\") = 'integer') , \"Home\" BOOL NOT NULL  DEFAULT 0, \"TimeLastUpdated\" DATETIME NOT NULL , \"TimeLastOnline\" DATETIME NOT NULL , \"Name\" VARCHAR NOT NULL , \"Online\" BOOL NOT NULL );";
    sql[4] = "DROP TABLE IF EXISTS \"Settings\";";
    sql[5] = "CREATE TABLE \"Settings\" (\"Name\" VARCHAR(10) UNIQUE,\"Value\" INTEGER);";
    sql[6] = "DROP TABLE IF EXISTS \"TrackedFolders\";";
    sql[7] = "CREATE TABLE \"TrackedFolders\" (\"Folderpath\" VARCHAR PRIMARY KEY  NOT NULL  UNIQUE);";
    sql[8] = "DROP TABLE IF EXISTS \"UserTable\";";
    sql[9] = "CREATE TABLE \"UserTable\" (\"ID\" INTEGER PRIMARY KEY  NOT NULL ,\"Artist\" VARCHAR,\"Album\" VARCHAR,\"Title\" VARCHAR,\"Rating\" INTEGER,\"Filename\" VARCHAR NOT NULL ,\"Year\" DATETIME,\"Length\" INTEGER,\"Bitrate\" INTEGER,\"Filesize\" INTEGER,\"Timestamp\" DATETIME NOT NULL ,\"Filetype\" VARCHAR,\"Deleted\" BOOL);";

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

QStringList Database::getFolders()
{
    QStringList folderlist;
    QSqlQuery result;
    QString sql;

    sql = "SELECT * FROM TrackedFolders ORDER BY Folderpath ASC;";

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

QString Database::lastUpdate(QString user_name)
{
    QString sql = "SELECT TimeLastUpdated FROM LibIndex WHERE Name=\"";
    sql += user_name;
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
        return r.value("TimeLastUpdated").toString();
    }
    catch(SBException e)
    {
        throw e;
    }

}
