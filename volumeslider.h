#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include <QSlider>

class VolumeSlider : public QSlider
{
    Q_OBJECT
public:
    explicit VolumeSlider(QWidget *parent = 0);
    QSlider*volumeslider;
    void get_widget();


signals:

public slots:

};

#endif // VOLUMESLIDER_H
