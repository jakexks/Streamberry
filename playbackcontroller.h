#ifndef PLAYBACKCONTROLLER_H
#define PLAYBACKCONTROLLER_H

#include "utilities.h"
#include "player.h"
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
};

#endif // PLAYBACKCONTROLLER_H
