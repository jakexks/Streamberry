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
    bool dbConnect(QString &path);
    bool dbInitialse();
    bool dbQuery(QString &statement);
    QSqlQuery dbSelectQuery(QString &statement);

private:
    QSqlDatabase db;
    bool connected;
};

#endif // DATABASE_H
