/* libVLC and Qt sample code
 * Copyright © 2009 Alexander Maringer <maringer@maringer-it.de>
 */
#include "vlc_on_qt.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <QFrame>
#include <QDialog>
#include <QMessageBox>
#include <iostream>

Player::Player()
    : QWidget()
{
    //preparation of the vlc command
    const char * const vlc_args[] = {
              "-I", "dummy", // Don't use any interface
              "--ignore-config", // Don't use VLC's config
              "--extraintf=logger", //log anything
              "--verbose=2", //be much more verbose then normal for debugging purpose
              "--plugin-path=C:\\vlc-0.9.9-win32\\plugins\\" };

    _videoWidget=new QFrame(this);

    _volumeSlider=new QSlider(Qt::Horizontal,this);
    _volumeSlider->setMaximum(100); //the volume is between 0 and 100
    _volumeSlider->setToolTip("Audio slider");

    // Note: if you use streaming, there is no ability to use the position slider
    _positionSlider=new QSlider(Qt::Horizontal,this);
    _positionSlider->setMaximum(POSITION_RESOLUTION);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(_videoWidget);
    layout->addWidget(_positionSlider);
    layout->addWidget(_volumeSlider);
    setLayout(layout);

    _isPlaying=false;
    poller=new QTimer(this);

    //Initialize an instance of vlc
    //a structure for the exception is neede for this initalization
    //libvlc_exception_init(&_vlcexcep);

    //create a new libvlc instance
    _vlcinstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);  //tricky calculation of the char space used
    //raise (&_vlcexcep);

    // Create a media player playing environement
    _mp = libvlc_media_player_new (_vlcinstance);
    //raise (&_vlcexcep);

    //connect the two sliders to the corresponding slots (uses Qt's signal / slots technology)
    connect(poller, SIGNAL(timeout()), this, SLOT(updateInterface()));
    connect(_positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(changePosition(int)));
    connect(_volumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(changeVolume(int)));

    poller->start(100); //start timer to trigger every 100 ms the updateInterface slot
}

//desctructor
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
    //the file has to be in one of the following formats /perhaps a little bit outdated)
    /*
    [file://]filename              Plain media file
    http://ip:port/file            HTTP URL
    ftp://ip:port/file             FTP URL
    mms://ip:port/file             MMS URL
    screen://                      Screen capture
    [dvd://][device][@raw_device]  DVD device
    [vcd://][device]               VCD device
    [cdda://][device]              Audio CD device
    udp:[[<source address>]@[<bind address>][:<bind port>]]
    */

    /* Create a new LibVLC media descriptor */
    _m = libvlc_media_new_location (_vlcinstance, file.toAscii());
    //raise(&_vlcexcep);

    libvlc_media_player_set_media (_mp, _m);
    libvlc_media_parse (_m);

    QMessageBox msgBox;
    msgBox.setText(libvlc_media_get_meta(_m, libvlc_meta_Title));
    msgBox.exec();
    //Free media
    libvlc_media_release (_m);
    //raise(&_vlcexcep);

    // /!\ Please note /!\
    //
    // passing the widget to the lib shows vlc at which position it should show up
    // vlc automatically resizes the video to the ´given size of the widget
    // and it even resizes it, if the size changes at the playing

    /* Get our media instance to use our window */
    #if defined(Q_OS_WIN)
        //libvlc_media_player_set_drawable(_mp, reinterpret_cast<unsigned int>(_videoWidget->winId()) );
        libvlc_media_player_set_hwnd(_mp, _videoWidget->winId() ); // for vlc 1.0
    #elif defined(Q_OS_MAC)
        //libvlc_media_player_set_drawable(_mp, _videoWidget->winId() );
        libvlc_media_player_set_agl (_mp, _videoWidget->winId()); // for vlc 1.0
    #else //Linux
        //libvlc_media_player_set_drawable(_mp, _videoWidget->winId() );
        libvlc_media_player_set_xwindow(_mp, _videoWidget->winId() ); // for vlc 1.0
    #endif
    //raise(&_vlcexcep);

    /* Play */
    libvlc_media_player_play (_mp);
    //raise(&_vlcexcep);

    _isPlaying=true;
}

/*void Player::changeVolume(int newVolume)
{
    //libvlc_audio_set_volume (_vlcinstance,newVolume);
}*/

void Player::changePosition(int newPosition)
{
    //libvlc_exception_clear(&_vlcexcep);
    // It's possible that the vlc doesn't play anything
    // so check before
    libvlc_media_t *curMedia = libvlc_media_player_get_media (_mp);
    //libvlc_exception_clear();
    if (curMedia == NULL)
        return;

    float pos=(float)(newPosition)/(float)POSITION_RESOLUTION;
    libvlc_media_player_set_position (_mp, pos);
    //raise(&_vlcexcep);
}

void Player::updateInterface()
{
    if(!_isPlaying)
        return;

    // It's possible that the vlc doesn't play anything
    // so check before
    libvlc_media_t *curMedia = libvlc_media_player_get_media (_mp);
    //libvlc_exception_clear(&_vlcexcep);
    if (curMedia == NULL)
        return;

    float pos=libvlc_media_player_get_position (_mp);
    int siderPos=(int)(pos*(float)(POSITION_RESOLUTION));
    _positionSlider->setValue(siderPos);
    int volume= 0;//libvlc_audio_get_volume (_vlcinstance);
    _volumeSlider->setValue(volume);
}
