#include "database.h"
#include <QtDebug>
#include <QtSql>
#include <QString>
#include "sbexception.h"
#include "crossplatform.h"

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

void Database::connect(QString &path)
{
    db.setDatabaseName(path);

    //if database can't be opened at path, print error and return with error code
    if(!db.open()) {
        throw SBException(DB, "Database could not be found.");
    } else {
        connected = true;
    }
}

void Database::initialse()
{
    QString path = CrossPlatform::getAppDataPath();
    path += "database.sqlite";

    //qDebug() << "Path is: " << path;

    //try to connect to database, if fail pass on exception
    try {
        connect(path);
    } catch (SBException e) {
        //need code here to create database
        throw e;
    }
}

void Database::query(QString sql)
{
    if(!connected) throw SBException(DB, "Cannot run query, not connected to database.");

    QSqlQuery query(db);
    query.prepare(sql);

    //qDebug() << sql;

    //if it can't execute, throw exception
    if(!query.exec()) {
        QString s = "SQL failed: ";
        s += query.lastError().text();
        throw SBException(DB, s);
    }
}

QSqlRecord Database::selectQuery(QString sql)
{
    if(!connected) throw SBException(DB, "Cannot run query, not connected to database.");

    QSqlQuery query(db);
    query.prepare(sql);

    //if it can't execute, throw exception
    if(!query.exec()) {
        QString s = "SQL failed: ";
        s += query.lastError().text();
        throw SBException(DB, s);
    }

    return query.record();
}
