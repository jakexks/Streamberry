#include "offlinechecker.h"
#include "utilities.h"
#include "beaconreceiver.h"

offlinechecker::offlinechecker(beaconreceiver &br) : rec(br)
{
}

void offlinechecker::detectOfflineMachines()
{
    QHash<QString, int>::const_iterator i;
    for (i = rec.onlineMachines.constBegin(); i != rec.onlineMachines.constEnd(); ++i)
    {
        if(i.value() - Utilities::getCurrentTimestamp() > 10)
        {
            //TODO: tell db machine is offline
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
