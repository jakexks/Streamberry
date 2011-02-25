#include "librarysender.h"
#include "networking.h"

LibrarySender::LibrarySender(Database &datab) : db(datab)
{

}

int LibrarySender::send(int timeStamp, QHostAddress sendTo, QString uniqueID)
{
    qDebug() << "sending library";
    networking n;
    QString sql = db.changesSinceTime(timeStamp, uniqueID);
    qDebug() << sql;
    n.send(sendTo, 5758, sql.toUtf8());
    return 0;
}
