#include "filemeta.h"
#include <QDebug>
#include <QFile>
#include <utilities.h>
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
    //commented out line below as it was causing a segfault
    //libvlc_media_release (_m);
    libvlc_release (_vlcinstance);
}

QList<QString> FileMeta::printMeta(QString file)
{
    QList<QString> meta;
    QString currmeta;

    mediaInstance = libvlc_media_new_location (_vlcinstance, file.toUtf8());
    libvlc_media_parse (mediaInstance);
    //currmeta = libvlc_media_get_meta(_m, libvlc_meta_Artist);
    //qDebug() << currmeta;
    //meta.add(libvlc_media_get_meta(_m, libvlc_meta_Title).toQString());
    //libvlc_media_get_meta(_m, libvlc_meta_)
    meta.append(currmeta.fromUtf8(libvlc_media_get_meta(mediaInstance, libvlc_meta_Artist)));
    meta.append(currmeta.fromUtf8(libvlc_media_get_meta(mediaInstance, libvlc_meta_Album)));
    meta.append(currmeta.fromUtf8(libvlc_media_get_meta(mediaInstance, libvlc_meta_Title)));
    meta.append(currmeta.fromUtf8(libvlc_media_get_meta(mediaInstance, libvlc_meta_Genre)));
    meta.append(currmeta.fromUtf8(libvlc_media_get_meta(mediaInstance, libvlc_meta_Rating)));
    meta.append(currmeta.fromUtf8(libvlc_media_get_meta(mediaInstance, libvlc_meta_Date)));
//    qDebug() << meta.at(2);
    //NO BITRATE
    //libvlc_media_player_get_length(); // length of video
    //libvlc_media_player_get_time(); // current position (milliseconds)*/
    /*libvlc_media_stats_t p_stats;
    //p_stats = new libvlc_media_stats_t;
    libvlc_media_get_stats(_m, &p_stats);
    //p_stats->i_read_bytes;
    qDebug() << p_stats.f_demux_bitrate;
    qDebug() << p_stats.i_decoded_audio;
    qDebug() << p_stats.i_read_bytes;
    //meta.append(currmeta.number(p_stats->i_read_bytes, 10));*/

    //////////////////////////////////////////MEDIA LENGTH ATTEMPT
    libvlc_media_player_t *mediaPlayerInstance;
    mediaPlayerInstance = libvlc_media_player_new_from_media(mediaInstance);
    libvlc_media_player_play (mediaPlayerInstance);
    if(libvlc_media_player_get_length(mediaPlayerInstance)==-1)
    {
        throw new SBException(GENERIC, "Invalid file");
    }
    int q=0;
    libvlc_audio_toggle_mute(mediaPlayerInstance);
    int length = libvlc_media_player_get_length(mediaPlayerInstance);
    while(length==0 )
    {
       if(q>500000)
         break;
       q++;
       length = libvlc_media_player_get_length(mediaPlayerInstance);
    }
    QString temp;
    temp.setNum(length);
    meta.append(temp);

    //qDebug() << time;
    //qDebug() << GetMetaInfo("Length");
    //meta.append(currmeta.number(libvlc_media_player_get_length(mediaPlayerInstance)/1000));


    ////////////////////////////////////////////////////////////////////////////////////////////////////


    meta.append(currmeta.fromUtf8(libvlc_media_get_meta(mediaInstance, libvlc_meta_TrackNumber)));
    libvlc_media_player_stop (mediaPlayerInstance);
    libvlc_media_player_release(mediaPlayerInstance);
    libvlc_media_release(mediaInstance);
    //QFile fileLoc(file);
    //qDebug() << fileLoc.size();
    //qDebug() << libvlc_media_get_duration(_m);
    //libvlc_media_player_get_length(_m);

    return meta;

}
