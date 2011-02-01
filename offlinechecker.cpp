#include "offlinechecker.h"
#include "utilities.h"
#include "beaconreceiver.h"
#include "database.h"

offlinechecker::offlinechecker(beaconreceiver &br, Database &datab) : rec(br), db(datab)
{
}

void offlinechecker::detectOfflineMachines()
{
    QHash<QString, int>::const_iterator i;
    for (i = rec.onlineMachines.constBegin(); i != rec.onlineMachines.constEnd(); ++i)
    {
        if(i.value() - Utilities::getCurrentTimestamp() > 10)
        {
            //tell db machine is offline
            db.setOnline(i.key(), "0");
            qDebug() << i.key() + "is offline";
            //remove the machine's ID from the hashtable of online machines
            rec.onlineMachines.remove(i.key());
        }
    }

}

//Checks if any machines have gone offline every 10 seconds
void offlinechecker::run()
{
    while(true)
    {
        detectOfflineMachines();
        sleep(10);
    }
}
