#ifndef PLAYBACKCONTROLLER_H
#define PLAYBACKCONTROLLER_H

#include "utilities.h"
#include <QtGui>

class PlaybackController
{
public:
    PlaybackController(Utilities& utilities);
    QWidget* getWidget();
    QWidget* makeWidget();
private:
    Utilities& util;
    QWidget* widget;
};

#endif // PLAYBACKCONTROLLER_H
