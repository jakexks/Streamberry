#include "librarysender.h"
#include "networking.h"

LibrarySender::LibrarySender(Database &datab) : db(datab)
{

}

int LibrarySender::send(int timeStamp, QHostAddress sendTo, QString uniqueID)
{
    networking n;
    QString sql = db.changesSinceTime(timeStamp, uniqueID);
    n.send(sendTo, 5758, sql.toUtf8());
    return 0;
}
