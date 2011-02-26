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
    QString currIP;
    QString remoteIP;
    libvlc_instance_t *_vlcinstance;
    //libvlc_exception_t *_vlcexcep;
    libvlc_media_player_t *_mp;
    libvlc_media_t *_m;

public:
    QTimer *poller;
    //void playFile(QString file);
    void playFile(QString file, QString uniqueID="local", QString ipaddress="local");
    Player();
    ~Player();

public slots:

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
