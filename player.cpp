#include "player.h"

#include <QTimer>
#include <QDebug>
#include <QPushButton>
#include <QMacCocoaViewContainer>
#include <QEvent>

#ifdef Q_OS_MAC
#include <Cocoa/Cocoa.h>
#endif

Player::Player()
{
    currIP = "";
    const char * const vlc_args[] = {
        "--no-video-title-show",
        //"--vout-event=0",
        //"-I", "dummy", /* Don't use any interface */
        //"--ignore-config", /* Don't use VLC's config */
        /*"--extraintf=logger", //log anything*/
        //"--verbose=1",
        //"--aout=pulse",
        //"--noaudio"
        //"--plugin-path=C:\\vlc-0.9.9-win32\\plugins\\"
#ifdef Q_OS_MAC
        "--vout=macosx"
#endif
            };
    _isPlaying=false;

    poller=new QTimer(this);
    _vlcinstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

    _mp = libvlc_media_player_new (_vlcinstance);
    connect(poller, SIGNAL(timeout()), this, SLOT(sliderUpdate()));
    poller->start(100);

    _videoWidget = NULL;
#ifdef Q_OS_MAC
    pool = [[NSAutoreleasePool alloc] init];
    videoView = [[NSView alloc] init];
    [videoView setAutoresizingMask: NSViewHeightSizable|NSViewWidthSizable];
#endif
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

#ifdef Q_OS_MAC
    [videoView release];
    [pool release];
#endif

    if(_videoWidget!=NULL)
        delete _videoWidget;
}

QWidget* Player::initVid()
{
    if(_videoWidget!=NULL)
        delete _videoWidget;

    frame = new QWidget();

#ifdef Q_OS_MAC
    _videoWidget = new QMacCocoaViewContainer(videoView, frame);
    _videoWidget->setFixedSize(frame->size());
    _videoWidget->show();
#endif

    frame->installEventFilter(this);

    return frame;
}

void Player::playFile(QString file, QString uniqueID, QString ipaddress)
{
    //Check if playing already, if remote, send stop
    //set up address if remote, send command to the other
    //if localplayback, give filename, if remote, set filename to 127.0.0.1
    //give filename normally

    if(currIP.compare("127.0.0.1")==0)
    {
        //Send command to other computer to stop. Use remoteIP variable
        QString toSend = "STREAMBERRY|STOP|";
        toSend += n.getuniqid();
        stream.send(remoteIP, 45459, toSend);
    }
    currIP = "Local"; //Change to local
    qDebug() << "Sending stream request to IP: " << ipaddress;

    if(ipaddress != "Local")
    {
        QString toSend = "";
        toSend += "STREAMBERRY|PLAY|";
        toSend += n.getmyip();
        toSend += "|";
        toSend += n.getuniqid();
        toSend += "|";
        toSend += file;
        toSend += "|";
        toSend += ipaddress;
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

    //Not sure if needed:
    //libvlc_media_release(libvlc_media_player_get_media(_mp));

    _m = libvlc_media_new_location (_vlcinstance, file.toUtf8());
    libvlc_media_player_set_media (_mp, _m);

#if defined(Q_OS_WIN)
    libvlc_media_player_set_hwnd(_mp, frame->winId());
#elif defined(Q_OS_MAC)
    libvlc_media_player_set_nsobject(_mp, videoView);
#else
    int windid = frame->winId();
    libvlc_media_player_set_xwindow (_mp, windid);
#endif
    libvlc_media_release (_m);
    libvlc_media_player_play (_mp);
    _isPlaying=true;

    emit play();
    //////////////////////////////////////////Links to preview
    oldtrack =0;
    emit playingalbumart();
    //////////////////////////////////////////////////////////
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
}

void Player::playControl()
{
    if(libvlc_media_player_get_media(_mp) == NULL)
    {
        emit getFirstSong(0);
    }

    if(currIP == "127.0.0.1")
    {
        QString tosend = "";
        tosend += "STREAMBERRY|PAUSE|";
        tosend += n.getuniqid();
        stream.send(remoteIP, 45459, tosend);
        if(_isPlaying==true)
        {
            _isPlaying=false;
            emit paused();
        }
        else {
            _isPlaying=true;
            emit play();
        }
    }
    else
    {
        if(libvlc_media_player_is_playing(_mp))
        {
            libvlc_media_player_set_pause(_mp, 1);
            emit paused();
        }
        else
        {
            libvlc_media_player_play(_mp);
            emit play();
        }
    }
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
    float pos;
    if(currIP == "127.0.0.1")
    {
        if(_isPlaying==true)
        {
            currSecs += (float)poller->interval();
        }
        pos = currSecs/fileLength;
    }
    else
    {
        pos=libvlc_media_player_get_position (_mp);
        setFileLength( libvlc_media_player_get_length(_mp) );
    }
    sliderPos=(int)(pos * (float)(POSITION_RESOLUTION));
    if(pos != 0)
        emit settrackprogress(pos);

    if(libvlc_media_player_get_state(_mp) == 6)//Stop if ended
    {
        libvlc_media_player_stop(_mp);

        emit getNextFile();
        sliderChanged(0);
        return;
    }

    libvlc_media_release(curMedia);
    if(sliderPos>5760)
    {
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
    if(secs != 0 && oldtrack == 0)
    {
        if(currIP == "127.0.0.1")
        {
            secs = secs*1000;
        }
        fileLength = secs + 2;
        oldtrack = 1;
        emit settracklength(fileLength);
    }
}

void Player::stopPlayer()
{
    libvlc_media_player_stop (_mp);
    _isPlaying=false;
    emit setAlbumArtDefault();
    emit paused();
}

bool Player::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == frame) {
        if(event->type() == QEvent::MouseButtonDblClick) {
            qDebug() << "Double clicked window";
        }
        return false;
    }
    else {
        return Player::eventFilter(obj, event);
    }
}
    
void Player::resizeVideo()
{
    //only necessary on Mac as other OSs resize
#if defined(Q_OS_MAC)
    _videoWidget->setFixedSize(frame->size());
#endif
}
