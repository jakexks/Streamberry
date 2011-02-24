#ifndef PLAYER_H
#define PLAYER_H

#include <vlc/vlc.h>
#include <QTimer>
#include <QObject>

#define POSITION_RESOLUTION 10000

class Player : public QObject
{
    Q_OBJECT;

private:
    bool _isPlaying;
    libvlc_instance_t *_vlcinstance;
    //libvlc_exception_t *_vlcexcep;
    libvlc_media_player_t *_mp;
    libvlc_media_t *_m;

public:
    QTimer *poller;
    Player();
    ~Player();

public slots:
    void playFile(QString file);
    void changePosition(int newPosition);
    void changeVolume(int newVolume);
    void playControl();
    void muteAudio();
    void sliderUpdate();
    void test();

signals:
    void sliderChanged(int newValue);

};

#endif // PLAYER_H
