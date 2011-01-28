#include "librarysend.h"
#include "database.h"
#include "networking.h"
#include <QHostAddress>

librarySend::librarySend(int timestamp, QString receiversID, Database &datab): db(datab)
{
    networking n;
    QString update_query = datab.changesSinceTime(timestamp,n.getuniqid());
    QString ip = n.parsebeacon(receiversID,networking::ip);
    QHostAddress host = QHostAddress::QHostAddress(ip);
    n.send(host,(quint16) 4310,update_query.toUtf8());
}
