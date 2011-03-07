#ifndef PLAYBACKBUTTON_H
#define PLAYBACKBUTTON_H

#include <QPushButton>
#include "playbackprogress.h"

class PlaybackButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PlaybackButton(PlaybackProgress *parent = 0);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
signals:

public slots:

private:
    void propagateEvent(QMouseEvent *e);
};

#endif // PLAYBACKBUTTON_H
