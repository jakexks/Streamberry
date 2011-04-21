#ifndef PLAYER_H
#define PLAYER_H

#include <vlc/vlc.h>
#include <QTimer>
#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QMacCocoaViewContainer>

#ifdef Q_WS_X11
    #include <QX11EmbedContainer>
#endif

#include "streamrequest.h"
#include "networking.h"

#define POSITION_RESOLUTION 5760

#ifdef Q_OS_MAC
    class NSView;
    class NSAutoreleasePool;
#endif

class Player : public QObject
{
    Q_OBJECT

private:
    bool _isPlaying;
    QString currIP;
    QString remoteIP;
    libvlc_instance_t *_vlcinstance;
    //libvlc_exception_t *_vlcexcep;
    libvlc_media_player_t *_mp;
    libvlc_media_t *_m;
    StreamRequest stream;
    networking n;
    int fileLength;
    float currSecs;
    QWidget* frame;
    //needed for video on Mac
    #ifdef Q_OS_MAC
        NSView* videoView;
        NSAutoreleasePool* pool;
    #endif
    QMacCocoaViewContainer *_videoWidget;
//    #ifdef Q_WS_X11
//        QX11EmbedContainer *_videoWidget;
//    #else
//        QFrame *_videoWidget;
//    #endif

public:
    QTimer *poller;
    //void playFile(QString file);
    void playFile(QString file, QString uniqueID="Local", QString ipaddress="Local");
    bool isPlaying();
    void setFileLength(int secs);
    QWidget* initVid();
    Player();
    ~Player();

public slots:

    void changePosition(int newPosition);
    void changeVolume(int newVolume);
    void playControl();
//    void muteAudio();
    void sliderUpdate();
    void test();

signals:
    void sliderChanged(int newValue);
    void getNextFile();
    void getFirstSong();
    void paused();
    void play();

};

#endif // PLAYER_H
