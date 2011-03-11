#include "volumeslider.h"
#include "QDebug"

VolumeSlider::VolumeSlider(QWidget *parent) :
    QSlider(parent)
{
    setOrientation(Qt::Horizontal);
    muted = false;
    lastposition = 75;
    setValue(lastposition);
    QObject::connect(this, SIGNAL(sliderReleased()), this, SLOT(updateLastPos()));
}

void VolumeSlider::muteVolSlider()
{
    if(muted)
    {
        setSliderPosition(lastposition);
        muted = false;
    }
    else
    {
        muted = true;
        setSliderPosition(0);
    }
}

void VolumeSlider::updateLastPos()
{
    if(sliderPosition()!=0)
    {
        lastposition = sliderPosition();
        muted = false;
    }
    else
    {
        muted = true;
    }
}
