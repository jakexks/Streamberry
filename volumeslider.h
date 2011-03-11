#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include <QSlider>

class VolumeSlider : public QSlider
{
    Q_OBJECT
public:
    explicit VolumeSlider(QWidget *parent = 0);
    QSlider*volumeslider;

private:
    int lastposition;
    bool muted;

signals:

public slots:
 void muteVolSlider();
 void updateLastPos();
};

#endif // VOLUMESLIDER_H
