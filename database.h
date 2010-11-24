#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QString>
#include <QStringList>

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
