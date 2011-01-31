#include "albuminfo.h"
#include <QString>
#include <QList>

QString albumTitle;
QString albumArtist;
QList<QString> tracks;

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
    tracks += "There She Goes";
    tracks += "Baddies Boogie";
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
