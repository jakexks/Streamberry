#include "player.h"

#include <QTimer>
#include <QDebug>
#include <QPushButton>
#include <QMacNativeWidget>

Player::Player()
{
    currIP = "";
    const char * const vlc_args[] = {
                  //"-I", "dummy", /* Don't use any interface */
                  "--ignore-config", /* Don't use VLC's config */
                  /*"--extraintf=logger", //log anything*/
                  //"--verbose=0",
                  //"--aout=pulse",
                  //"--noaudio"
                  //"--plugin-path=C:\\vlc-0.9.9-win32\\plugins\\"
                  //"--vout=agl"
              };
    _isPlaying=false;

    poller=new QTimer(this);
    _vlcinstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

    _mp = libvlc_media_player_new (_vlcinstance);
    connect(poller, SIGNAL(timeout()), this, SLOT(sliderUpdate()));
    //connect(_positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(changePosition(int)));
    //connect(_volumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(changeVolume(int)));
    poller->start(100);
}

Player::~Player()
{
    // Stop playing
    libvlc_media_player_stop (_mp);

    // Free the media_player
    libvlc_media_player_release (_mp);


    libvlc_release (_vlcinstance);
    //raise (&_vlcexcep);

    poller->stop();
    delete poller;
}

QWidget* Player::initVid()
{
    frame = new QFrame();

//    #ifdef Q_WS_X11
//        _videoWidget = new QX11EmbedContainer(frame);
//    #else
//        _videoWidget=new QFrame(frame);
//    #endif
    return frame;
}

/*void Player::playFile(QString file)
{
    _m = libvlc_media_new_location (_vlcinstance, file.toUtf8());
    libvlc_media_player_set_media (_mp, _m);
    //libvlc_media_parse (_m);
    libvlc_media_release (_m);
    libvlc_media_player_play (_mp);
    _isPlaying=true;
    currIP="";//Set IP to empty
}*/

void Player::playFile(QString file, QString uniqueID, QString ipaddress)
{
    //Check if playing already, if remote, send stop
    //set up address if remote, send command to the other
    //if localplayback, give filename, if remote, set filename to 127.0.0.1
    //give filename normally

    if(currIP == "127.0.0.1")
    {
        //Send command to other computer to stop. Use remoteIP variable
        QString toSend = "STREAMBERRY|STOP|";
        toSend += n.getuniqid();
        stream.send(remoteIP, 45459, toSend);
    }

    currIP = "Local"; //Change to local
    qDebug() << ipaddress;

    if(ipaddress != "Local")
    {
        QString toSend = "";
        toSend += "STREAMBERRY|PLAY|";
        toSend += n.getmyip();
        toSend += "|";
        toSend += n.getuniqid();
        toSend += "|";
        toSend += file;
        //Send IP, uniqueID, file path
        stream.send(ipaddress, 45459, toSend);
        file = "rtp://@";
        #ifdef Q_WS_WIN
            file = "rtp://";
            file += n.getmyip();
            file += ":5004";
        #endif
        qDebug() << file;
        currIP = "127.0.0.1";
        remoteIP = ipaddress;
        currSecs = 0;
    }


    libvlc_media_release(libvlc_media_player_get_media(_mp));

    _m = libvlc_media_new_location (_vlcinstance, file.toUtf8());
    libvlc_media_player_set_media (_mp, _m);
    //libvlc_media_parse (_m);

    #if defined(Q_OS_WIN)
        libvlc_media_player_set_drawable(_mp, reinterpret_cast<unsigned int>(frame->winId()));
    #elif defined(Q_OS_MAC)
        //libvlc_media_player_set_agl(_mp, frame->winId());
//        int view = frame->winId();
//        libvlc_media_player_set_nsobject(_mp, (void*)view);
        frame->setStyleSheet("background: magenta;");

//        NSView* videoView = [[NSView alloc] init];

//        QMacCocoaViewContainer *_videoWidget = new QMacCocoaViewContainer(videoView, frame);

//        //[videoView setAutoresizingMask: NSViewHeightSizable|NSViewWidthSizable];
//        libvlc_media_player_set_nsobject(_mp, frame);

    #else
        int windid = frame->winId();
        libvlc_media_player_set_xwindow (_mp, windid);
    #endif
    libvlc_media_release (_m);
    libvlc_media_player_play (_mp);
    _isPlaying=true;
    emit play();

    /*if(remote)
    {
        if(libvlc_media_player_is_playing)
        {
            libvlc_media_player_stop(_mp);
        }

    } else {
        playFile(file);
    }*/
}

void Player::changeVolume(int newVolume)
{
    libvlc_audio_set_volume (_mp,newVolume);
}

void Player::changePosition(int newPosition)
{
    libvlc_media_t *curMedia = libvlc_media_player_get_media (_mp);
    if (curMedia == NULL)
    {
        qDebug() << "No media loaded";
        return;
    }
    libvlc_media_release(curMedia);

    float pos = (float)(newPosition)/(float)POSITION_RESOLUTION;
    if(pos > 0.99 )
    {
        pos = 0.9999;
        libvlc_media_player_stop(_mp);
    }
    if(currIP == "127.0.0.1")
    {
        QString tosend = "";
        tosend += "STREAMBERRY|SEEK|";
        tosend += n.getuniqid();
        tosend += "|";
        tosend += QString::number(pos);
        qDebug() << "SEEKING TO: " << pos;
        stream.send(remoteIP, 45459, tosend);
        currSecs = pos*fileLength;
    } else {
        libvlc_media_player_set_position (_mp, pos);
    }
    //libvlc_media_player_play(_mp);
}

void Player::playControl()
{
    if(libvlc_media_player_get_media(_mp) == NULL)
    {
        emit getFirstSong();
    }

    if(currIP == "127.0.0.1")
    {
        QString tosend = "";
        tosend += "STREAMBERRY|PAUSE|";
        tosend += n.getuniqid();
        stream.send(remoteIP, 45459, tosend);
    } else {
        if(libvlc_media_player_is_playing(_mp))
        {
            libvlc_media_player_set_pause(_mp, 1);
            emit paused();
        } else {
            libvlc_media_player_play(_mp);
            emit play();
        }
    }
}

//void Player::muteAudio()
//{
//    libvlc_audio_toggle_mute(_mp);
//}

void Player::test(){
    qDebug() << "\ntest\n";
}

void Player::sliderUpdate()
{
    if(!_isPlaying)
            return;

    libvlc_media_t *curMedia = libvlc_media_player_get_media (_mp);
    if (curMedia == NULL)
    {
        qDebug() << "No media loaded";
        return;
    }

    int sliderPos;
    if(currIP == "127.0.0.1")
    {
        currSecs += (float)poller->interval()/1000;
        float pos = currSecs/fileLength;
        sliderPos = (int)(pos * (float)(POSITION_RESOLUTION));
    } else {
        float pos=libvlc_media_player_get_position (_mp);
        sliderPos=(int)(pos * (float)(POSITION_RESOLUTION));
    }

    if(libvlc_media_player_get_state(_mp) == 6)//Stop if ended
    {
        libvlc_media_player_stop(_mp);
        //TODO: Check if on loop
        emit getNextFile();
    }
    sliderChanged(sliderPos);
}

bool Player::isPlaying()
{
    return libvlc_media_player_is_playing(_mp);
}

void Player::setFileLength(int secs)
{
    fileLength = secs;
}
