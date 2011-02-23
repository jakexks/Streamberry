#include "filemeta.h"
#include <QDebug>
#include <QFile>
FileMeta::FileMeta()
{
    const char* vlc_args[] = {
                  "--ignore-config",
                  "--verbose=0"
                      };
    _vlcinstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
}

FileMeta::~FileMeta()
{
    libvlc_media_release (_m);
    libvlc_release (_vlcinstance);
}

QList<QString> FileMeta::printMeta(QString file)
{
    QList<QString> meta;
    QString currmeta;
    _m = libvlc_media_new_location (_vlcinstance, file.toAscii());
    libvlc_media_parse (_m);

    //currmeta = libvlc_media_get_meta(_m, libvlc_meta_Artist);
    //qDebug() << currmeta;
    //meta.add(libvlc_media_get_meta(_m, libvlc_meta_Title).toQString());
    //libvlc_media_get_meta(_m, libvlc_meta_)
    meta.append(currmeta.fromAscii(libvlc_media_get_meta(_m, libvlc_meta_Artist)));
    meta.append(currmeta.fromAscii(libvlc_media_get_meta(_m, libvlc_meta_Album)));
    meta.append(currmeta.fromAscii(libvlc_media_get_meta(_m, libvlc_meta_Title)));
    meta.append(currmeta.fromAscii(libvlc_media_get_meta(_m, libvlc_meta_Genre)));
    meta.append(currmeta.fromAscii(libvlc_media_get_meta(_m, libvlc_meta_Rating)));
    meta.append(currmeta.fromAscii(libvlc_media_get_meta(_m, libvlc_meta_Date)));
    //NO BITRATE
    /*libvlc_media_player_get_length(); // length of video
    libvlc_media_player_get_time(); // current position (milliseconds)*/
    /*libvlc_media_stats_t p_stats;
    //p_stats = new libvlc_media_stats_t;
    libvlc_media_get_stats(_m, &p_stats);
    //p_stats->i_read_bytes;
    qDebug() << p_stats.f_demux_bitrate;
    qDebug() << p_stats.i_decoded_audio;
    qDebug() << p_stats.i_read_bytes;
    //meta.append(currmeta.number(p_stats->i_read_bytes, 10));*/
    libvlc_media_player_t *_mp;
    _mp = libvlc_media_player_new_from_media(_m);
    libvlc_media_player_play (_mp);
    while(libvlc_media_player_get_length(_mp)==0){
        //sleep(1);
    }
    //sleep(1);
    //currmeta.f
    //currmeta.number(libvlc_media_player_get_length(_mp)/1000);
    meta.append(currmeta.number(libvlc_media_player_get_length(_mp)/1000));
    libvlc_media_player_stop (_mp);
    libvlc_media_player_release(_mp);
    libvlc_media_release(_m);
    //QFile fileLoc(file);
    //qDebug() << fileLoc.size();
    //qDebug() << libvlc_media_get_duration(_m);
    //libvlc_media_player_get_length(_m);

    return meta;

}
