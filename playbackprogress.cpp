#include "playbackprogress.h"
#include "utilities.h"
#include <QtGui>
#include <cmath>

PlaybackProgress::PlaybackProgress(Utilities &utilities, QWidget *parent) : QFrame(parent), util(utilities)
{
   // setStyleSheet("#progressback {background:url(/home/byalkov/Documents/Streamberry-build-desktop/images/botBarProgress.png); background-repeat:none} #playbutton {background-color:rbg(0,0,0,0); background-repeat:none; background-image:url(/home/byalkov/Documents/Streamberry-build-desktop/images/botBarPlayUp.png) }");
    setStyleSheet(util.getStylesheet());
    setObjectName("progressback");
    setFixedSize(82, 83);
    pieangle = 0;
}

void PlaybackProgress::mousePressEvent(QMouseEvent *e)
{
    grabMouse();
    recalculateAngle(e->posF());
}

void PlaybackProgress::mouseMoveEvent(QMouseEvent *e)
{
    recalculateAngle(e->posF());
}

void PlaybackProgress::mouseReleaseEvent(QMouseEvent *e)
{
    releaseMouse();
}

void PlaybackProgress::recalculateAngle(QPointF ptc)
{
    QPointF pta(0, 0);
    QPointF ptb(0, -25);
    ptc.setX(ptc.x()-41);
    ptc.setY(ptc.y()-41);

    //work out the distances between each point
    float lenc = 25;
    float lenb = sqrt(pow(ptc.x()-pta.x(), 2.0) + pow(ptc.y()-pta.y(), 2.0));
    float lena = sqrt(pow(ptc.x()-ptb.x(), 2.0) + pow(ptc.y()-ptb.y(), 2.0));
    float angle = 0.0;

    //if the point is on the y axis, work out if angle is 0 or 180
    if(ptc.x()==0)
    {
        angle = (ptc.y()>=0) ? 180.0 : 0.0;
    }
    else if(ptc.x()<0)
    {
        //cos(A) = (b^2 + c^2 - a^2) / 2*b*c
        float cosa = (lenb*lenb + lenc*lenc - lena*lena) / (2*lenb*lenc);
        angle = acos(cosa) * 180.0 / 3.14159265;
        angle = 360 - angle;
    }
    else
    {
        //cos(A) = (b^2 + c^2 - a^2) / 2*b*c
        float cosa = (lenb*lenb + lenc*lenc - lena*lena) / (2*lenb*lenc);
        angle = acos(cosa) * 180.0 / 3.14159265;
    }

    pieangle = (int)(angle*16);
    update();
}

void PlaybackProgress::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(80, 192, 224));

    painter.save();
    painter.drawPie(2, 2, 78, 78, 16*90, -pieangle);
    painter.restore();
}
