#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QString>
#include <QStringList>
#include <QSqlRecord>
#include <QList>

using namespace std;

class Database
{
public:
    Database();
    virtual ~Database();
    //initialises the database
    void initialise();
    //updates or adds setting to database. please do not make typos in parameters, it would mess up the db.
    void storeSetting(QString name, QString value);
    //returns value of setting given to name parameter. returns NULL if not found.
    QString getSetting(QString name);
    //set list of folders for file scanner to search through. takes semi-colon separated string.
    //note: clears table first, so any folders not in this list will be removed!
    void setFolders(QString folders);
    //takes QStringList instead of string
    void setFolders(QStringList folderlist);
    //get list of folders for file scanner to search through. returns QStringList containing folders.

    QStringList getFolders();
    //gets the last update of the library of a specified user
    QString lastUpdate(QString user_name);
    //get all changes from Local after timestamp. takes this computer's unique ID.
    QString changesSinceTime(int timestamp, QString uniqueID);

    //if argument 0 gets tracked, if 1 gets excluded
    QStringList getFolders(int trackedOrExcluded);
    //counts the number of rows in a table
    int rowCount(QString tablename);
    //adds file to database. if already in database it just updates.
    void addFile(QString filepath, QString filename, QString filesize, QString artist, QString album, QString title, QString genre, QString rating, QString year, QString length, QString bitrate, QString filetype, QString table);
    //deletes file by ID. returns number of files deleted.
    int deleteFile(QString id, QString table);
    //perform search.
    //type: 0 - all, 1 - artist, 2 - title, 3 - genre
    //return order: Filepath, Artist, Album , Title , Genre, Rating , Filename , Year , Length , Bitrate , Filesize , Timestamp , Filetype
    QList<QSqlRecord> searchDb(int type, QString searchtxt);


private:
    //connects to database in given path
    void connect(QString &path);
    //creates database file
    void createDatabase(QString &path);
    //performs an SQL query
    QSqlQuery query(QString sql);

    QSqlDatabase db;
    //says whether database is connected or not
    bool connected;
    //file name not including path
    QString dbfilename;
};

#endif // DATABASE_H
