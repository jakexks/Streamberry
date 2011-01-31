#ifndef ALBUMINFO_H
#define ALBUMINFO_H
#include <QList>
#include <QString>
class albuminfo
{
public:
    albuminfo();
    QList<QString> getTracks();
    QString getArtist();
    QString getTitle();
private:
    QString albumTitle;
    QString albumArtist;
    QList<QString> tracks;
};

#endif // ALBUMINFO_H
