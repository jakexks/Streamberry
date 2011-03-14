#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include <QSlider>
#include "utilities.h"

class VolumeSlider : public QSlider
{
    Q_OBJECT
public:
    explicit VolumeSlider(Utilities& utilities, QWidget *parent = 0);
    QSlider*volumeslider;

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
