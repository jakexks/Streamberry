#ifndef PLAYBACKCONTROLLER_H
#define PLAYBACKCONTROLLER_H

#include "utilities.h"
#include "player.h"
#include "volumeslider.h"
#include "songinfo.h"
#include <QtGui>

class PlaybackController : public QObject
{
    Q_OBJECT;
public:
    PlaybackController(Utilities& utilities, Player& p);
    QWidget* getWidget();
    QWidget* makeWidget();
    SongInfo* getSongInfo();
private:
    Utilities& util;
    Player& player;
    QWidget* widget;
    QPushButton *mute;
    QPushButton *shuffle;
    QPushButton *repeat;
    VolumeSlider *volumeslider;
    SongInfo *songinfoarea;
    bool shufflePressed;
    bool repeatOnePressed;
    bool repeatAllPressed;

signals:
    void nextFile();
    void prevFile();
    void muteButtonDown();
    void sPress(); //shuffle pressed
    void rPress(bool i, bool j); //repeat pressed

public slots:
    void muteSlider();
    void shufflePress();
    void repeatPress();
};

#endif // PLAYBACKCONTROLLER_H
