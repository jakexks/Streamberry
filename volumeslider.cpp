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

    QObject::connect(this, SIGNAL(valueChanged(int)), this, SLOT(updateLastPos()));

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

int VolumeSlider::getlastpos()
{
  return lastposition;
}

void VolumeSlider::setslidepos(int j)
{
  setSliderPosition(j);
}


void VolumeSlider::updateLastPos()
{

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
    if(sliderPosition()<4)
    {
        setStyleSheet("#bottomBarVolumeslider:sub-page:horizontal {background-color:#50c1e0; margin:1px 0px 2px 4px; height:10px; border-radius:4px; }");
    }
    if(sliderPosition()<2)
    {
        setStyleSheet("#bottomBarVolumeslider:sub-page:horizontal {background-color:#50c1e0; margin:1px 0px 2px 4px; height:10px; border-radius:3px; }");
    }

}
