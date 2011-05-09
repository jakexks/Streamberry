#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include <QSlider>
#include "utilities.h"

//The volume slider in the bottom bar
class VolumeSlider : public QSlider
{
    Q_OBJECT
public:
    explicit VolumeSlider(Utilities& utilities, QWidget *parent = 0);
    QSlider*volumeslider;
    //returns the last position of the slider before being muted
    int getlastpos();
    //sets the slider to this position
    void setslidepos(int j);

private:
    int lastposition;
    bool muted;
    Utilities& util;

signals:

public slots:
 void muteVolSlider();
 void updateLastPos();
};

#endif // VOLUMESLIDER_H
