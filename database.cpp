#include "database.h"
#include <QtDebug>
#include <QtSql>
#include <QString>
#include "sbexception.h"

using namespace std;

Database::Database()
{
    db = QSqlDatabase::addDatabase( "QSQLITE" );
    connected = false;
    //qDebug() << "Constructor called";
}

//deconstructor
Database::~Database()
{
    //if db is connected then it must be closed
    if(connected) {
        db.close();
    }
}

void Database::dbConnect(QString &path)
{
    db.setDatabaseName(path);

    //if database can't be opened at path, print error and return with error code
    if(!db.open()) {
        throw SBException(DB, "Database could not be found.");
    } else {
        connected = true;
    }
}

void Database::dbInitialse()
{
    QString path = "./database.sqlite";

    //try to connect to database, if fail pass on exception
    try {
        dbConnect(path);
    } catch (SBException e) {
        throw e;
    }
}

void Database::dbQuery(QString &sql)
{
    QSqlQuery query;
    query.prepare(sql);

    if(!query.exec()) {
        QString s = "SQL failed: ";
        s += query.lastError().text();
        throw SBException(DB, s);
    }
}

QSqlRecord Database::dbSelectQuery(QString &sql)
{
    QSqlQuery query;
    query.prepare(sql);

    if(!query.exec()) {
        QString s = "SQL failed: ";
        s += query.lastError().text();
        throw SBException(DB, s);
    }

    return query.record();
}
