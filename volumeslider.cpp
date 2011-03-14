#include "volumeslider.h"
#include "QDebug"

VolumeSlider::VolumeSlider(Utilities &utilities, QWidget *parent) :
        QSlider(parent), util(utilities)
{
    setOrientation(Qt::Horizontal);
    setObjectName("bottomBarVolumeslider");
    muted = false;
    lastposition = 75;
    setValue(lastposition);
    QObject::connect(this, SIGNAL(sliderReleased()), this, SLOT(updateLastPos()));


}

void VolumeSlider::muteVolSlider()
{
    if(muted)
    {
        setObjectName("bottomBarVolumeslider");
        setStyleSheet(util.getStylesheet());
        setSliderPosition(lastposition);
        muted = false;
    }
    else
    {

        if(sliderPosition()<20)
        {
            setObjectName("bottomBarVolumesliderMuted");
            setStyleSheet(util.getStylesheet());
        }
        muted = true;
        setSliderPosition(0);
        setObjectName("bottomBarVolumesliderMuted");
        setStyleSheet(util.getStylesheet());
    }
}

void VolumeSlider::updateLastPos()
{
    if(sliderPosition()<20)
    {
        setObjectName("bottomBarVolumesliderMuted");
        setStyleSheet(util.getStylesheet());
    }
    if(sliderPosition()!=0)
    {
        lastposition = sliderPosition();
        setObjectName("bottomBarVolumeslider");
        setStyleSheet(util.getStylesheet());
        muted = false;
    }
    else
    {
        muted = true;
        setObjectName("bottomBarVolumesliderMuted");
        setStyleSheet(util.getStylesheet());
    }
}
