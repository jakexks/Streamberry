#ifndef FILEMETA_H
#define FILEMETA_H

#include <QString>
#include <QList>
#include <vlc/vlc.h>

class FileMeta
{
    libvlc_instance_t *_vlcinstance;
    libvlc_media_t *_m;
public:
    FileMeta();
    ~FileMeta();

    QList<QString> printMeta(QString path);
};

#endif // FILEMETA_H
