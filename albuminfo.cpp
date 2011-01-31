#include "albuminfo.h"
#include <QString>
#include <QList>

QString albumTitle;
QString artist;
QList<QString> tracks;

albuminfo::albuminfo()
{
    // Sample data...
    albumTitle = "Shotter's Nation";
    artist = "Babyshambles";
    tracks += "Delivery";
    tracks += "You Talk";
    tracks += "UnBiloTitled";
    tracks += "Side of the Road";
    tracks += "Crumb Begging Baghead";
    tracks += "Unstookie Titled";
    tracks += "French Dog Blues";
    tracks += "There SHe Goes";
    tracks += "Baddies Boogiex";
    tracks += "Deft Left Hand";
    tracks += "Lost Art of Murder";
}
