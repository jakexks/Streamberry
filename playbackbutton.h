#ifndef PLAYBACKBUTTON_H
#define PLAYBACKBUTTON_H

#include <QPushButton>
#include "playbackprogress.h"

//Play/pause button in playback controls
class PlaybackButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PlaybackButton(Utilities& utilities, PlaybackProgress *parent = 0);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
signals:

public slots:
    //changes the name of the object in order to use different CSS style
    void changeNamePlay();
    void changeNamePause();

private:
    void propagateEvent(QMouseEvent *e);
    Utilities& util;
};

#endif // PLAYBACKBUTTON_H
