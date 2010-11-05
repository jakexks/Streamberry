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
    void dbInitialse();
    //performs an action type SQL query
    void dbQuery(QString &sql);
    //performs a get type SQL query and returns records
    QSqlRecord dbSelectQuery(QString &sql);

private:
    //connects to database in given path
    void dbConnect(QString &path);

    QSqlDatabase db;
    //says whether database is connected or not
    bool connected;
};

#endif // DATABASE_H
