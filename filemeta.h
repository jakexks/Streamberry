#ifndef FILEMETA_H
#define FILEMETA_H

#include <QString>
#include <QList>
#include <vlc/vlc.h>
#include "sbexception.h"

//gets tags from files
class FileMeta
{
    libvlc_instance_t *_vlcinstance;
    libvlc_media_t *mediaInstance;
public:
    FileMeta();
    ~FileMeta();

    QList<QString> printMeta(QString path);
};

#endif // FILEMETA_H
