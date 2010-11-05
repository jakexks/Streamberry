#include "database.h"
#include <QtDebug>
#include <QtSql>
#include <QString>

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

bool Database::dbConnect(QString &path)
{
    db.setDatabaseName(path);

    //if database can't be opened at path, print error and return with error code
    if(!db.open()) {
        qFatal("Database could not be found.");
        return false;
    } else {
        connected = true;
    }

    return true;
}

bool Database::dbInitialse()
{
    QString path = "./database.sqlite";

    if(!dbConnect(path)) {
        qFatal("Database unable to be initialised.");
        return false;
    }

    return true;
}

bool Database::dbQuery(QString &statement)
{
    QSqlQuery query;
    query.prepare(statement);

    if(!query.exec()) {
        qDebug() << "SQL statement failed: " << query.lastError();
        return false;
    }

    return true;
}

QSqlQuery Database::dbSelectQuery(QString &statement)
{
    QSqlQuery query;
    query.prepare(statement);

    if(!query.exec()) {
        qDebug() << "SQL statement failed: " << query.lastError();
        return NULL;
    }

    return query;
}
