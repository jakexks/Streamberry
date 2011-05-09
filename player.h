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


//Interfaces with VLC and allows you to control ongoing media
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
    int oldtrack;
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
    //Function that starts playing the file given. UniqueID, ipaddress of the remote computer
    void playFile(QString file, QString uniqueID="Local", QString ipaddress="Local");
    //Check if something is playing locally. Will return 1 if listening to a stream
    bool isPlaying();
    //Set the length of the file - used when listening to remote file
    void setFileLength(int secs);
    void stopPlayer();
    //Initialise the frame output - returns pointer to QWidget
    QWidget* initVid();
    void resizeVideo();
    Player();
    ~Player();

public slots:
    //Seek to position newPosition. Max value is 5760
    void changePosition(int newPosition);
    void changeVolume(int newVolume);
    //Play or pause depending on current state
    void playControl();
    //Called by the poller to update the progress
    void sliderUpdate();

protected:
    //Event handler - can be used to implement full screen video
     bool eventFilter(QObject *obj, QEvent *ev);

signals:
    //Updated progress value
    void sliderChanged(int newValue);
    //Called at the end of the file to request the next one
    void getNextFile();
    void getFirstSong(const int i);
    void paused();
    void play();
    void playingalbumart();
    //Set the length of the progress bar
    void settracklength(int);
    void settrackprogress(float);
    void setAlbumArtDefault();
    //Signal to tell the GUI to change to video view
    void isvideo();
};

#endif // PLAYER_H
