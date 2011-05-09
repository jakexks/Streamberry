#ifndef PLAYBACKCONTROLLER_H
#define PLAYBACKCONTROLLER_H

#include "utilities.h"
#include "player.h"
#include "volumeslider.h"
#include "songinfo.h"
#include <QtGui>

//Controls the playback controls
class PlaybackController : public QObject
{
    Q_OBJECT;
public:
    PlaybackController(Utilities& utilities, Player& p);
    QWidget* getWidget();
    QWidget* makeWidget();
    //returns the songinfo widget
    SongInfo* getSongInfo();
    //returns the position of the volume slider
    int getvolpos();
    //move the volume slider to this position
    void setvolumepos(int j);

private:
    Utilities& util;
    Player& player;
    QWidget* widget;
    QPushButton *mute;
    QPushButton *shuffle;
    QPushButton *repeat;
    VolumeSlider *volumeslider;
    SongInfo *songinfoarea;
    //booleans for the states of the buttons
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
    //sets the slider to 0 if the mute button is pressed
    void muteSlider();
    //works out the state of the shuffle button (2 states)
    void shufflePress();
    //works out the state of the repeat button (3 states)
    void repeatPress();
};

#endif // PLAYBACKCONTROLLER_H
