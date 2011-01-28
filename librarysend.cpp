#include "librarysend.h"
#include "database.h"
#include "networking.h"

librarySend::librarySend(int timestamp, QString receiversID, Database &datab): db(datab)
{
    networking n;
    QString update_query = datab.changesSinceTime(timestamp,n.getuniqid());
    QString ip = n.parsebeacon(receiversID,networking::ip);
    n.send(ip,4310,update_query.toUtf8());
}
