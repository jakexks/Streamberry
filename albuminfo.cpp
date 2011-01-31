#include "albuminfo.h"
#include <QString>
#include <QList>

albuminfo::albuminfo()
{
    // Sample data...
    albumTitle = "Shotter's Nation";
    albumArtist = "Babyshambles";
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
QString albuminfo::getArtist()
{
    return albumArtist;
}

QString albuminfo::getTitle()
{
    return albumTitle;
}

QList<QString> albuminfo::getTracks()
{
    return tracks;
}
