#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QString>

using namespace std;

class Database
{
public:
    Database();
    virtual ~Database();
    //initialises the database
    void initialise();
    //performs an action type SQL query
    void query(QString sql);
    //performs a get type SQL query and returns records
    QSqlRecord selectQuery(QString sql);

private:
    //connects to database in given path
    void connect(QString &path);
    //creates database file
    void createDatabase(QString &path);

    QSqlDatabase db;
    //says whether database is connected or not
    bool connected;
    QString dbfilename;
};

#endif // DATABASE_H
