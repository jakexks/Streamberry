#include "player.h"

#include <QTimer>
#include <QDebug>

Player::Player()
{
    const char * const vlc_args[] = {
                  "-I", "dummy", /* Don't use any interface */
                  "--ignore-config", /* Don't use VLC's config */
                  "--extraintf=logger", //log anything
                  "--verbose=0", //be much more verbose then normal for debugging purpose
                  //"--plugin-path=C:\\vlc-0.9.9-win32\\plugins\\"
              };
    _isPlaying=false;

    poller=new QTimer(this);
    _vlcinstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

    _mp = libvlc_media_player_new (_vlcinstance);

    poller->start(100);
    //connect(_positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(changePosition(int)));
    //connect(_volumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(changeVolume(int)));
}

Player::~Player()
{
    // Stop playing
    libvlc_media_player_stop (_mp);

    // Free the media_player
    libvlc_media_player_release (_mp);


    libvlc_release (_vlcinstance);
    //raise (&_vlcexcep);
}

void Player::playFile(QString file)
{
    _m = libvlc_media_new_location (_vlcinstance, file.toAscii());
    libvlc_media_player_set_media (_mp, _m);
    //libvlc_media_parse (_m);
    libvlc_media_release (_m);
    libvlc_media_player_play (_mp);
    _isPlaying=true;
}

void Player::changeVolume(int newVolume)
{
    libvlc_audio_set_volume (_mp,newVolume);
}

void Player::changePosition(int newPosition)
{
    libvlc_media_t *curMedia = libvlc_media_player_get_media (_mp);
    if (curMedia == NULL)
        return;

    float pos=(float)(newPosition)/(float)POSITION_RESOLUTION;
    libvlc_media_player_set_position (_mp, pos);
}

void Player::playControl()
{
    libvlc_media_player_pause(_mp);
    /*if(play == true){
        libvlc_media_player_set_pause(_mp, 0);
        _isPlaying=true;
    } else {
        libvlc_media_player_set_pause(_mp, 1);
        _isPlaying=false;
    }*/
}

void Player::test(){
    qDebug() << "\ntest\n";
}
