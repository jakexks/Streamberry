#ifndef FILE_META_H
#define FILE_META_H

#include <QString>
#include <vlc/vlc.h>

class File_Meta
{
    libvlc_instance_t *_vlcinstance;
    libvlc_media_t *_m;

public:
    File_Meta();
    ~File_Meta();

public:
    void print_meta(QString file);
};

#endif // FILE_META_H
