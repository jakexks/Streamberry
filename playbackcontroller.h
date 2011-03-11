#ifndef PLAYBACKCONTROLLER_H
#define PLAYBACKCONTROLLER_H

#include "utilities.h"
#include "player.h"
#include "volumeslider.h"
#include <QtGui>

class PlaybackController : public QObject
{
    Q_OBJECT;
public:
    PlaybackController(Utilities& utilities, Player& p);
    QWidget* getWidget();
    QWidget* makeWidget();
private:
    Utilities& util;
    Player& player;
    QWidget* widget;
    QPushButton *mute;
    VolumeSlider *volumeslider;

signals:
    void nextFile();
    void prevFile();
    void muteButtonDown();

public slots:
    void muteSlider();
};

#endif // PLAYBACKCONTROLLER_H
