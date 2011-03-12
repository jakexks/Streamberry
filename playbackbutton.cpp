#include "playbackbutton.h"
#include <QtGui>
#include "playbackprogress.h"
#include <QDebug>
#include "utilities.h"

PlaybackProgress *prog;

PlaybackButton::PlaybackButton(Utilities &utilities,PlaybackProgress *parent) :
        QPushButton(parent), util(utilities)
{
    prog = parent;
    setGeometry(10, 11, 65, 65);
    setFlat(true);
    setObjectName("bottomBarPlaypause");
    //    setVisible(false);
}

void PlaybackButton::changeNamePlay()
{
        setObjectName("pausebutton");
        setStyleSheet(util.getStylesheet());
}
void PlaybackButton::changeNamePause()
{
        setObjectName("playbutton");
        setStyleSheet(util.getStylesheet());
}


void PlaybackButton::mousePressEvent(QMouseEvent *e)
{
    propagateEvent(e);
}

void PlaybackButton::mouseMoveEvent(QMouseEvent *e)
{
    propagateEvent(e);
}

void PlaybackButton::propagateEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton || e->type()==QEvent::MouseMove)
    {
        QPointF p = e->posF();

        float xclick = p.x()-(float)(68/2);
        float yclick = p.y()-(float)(65/2);

        //if the click was within the circle
        if(xclick*xclick + yclick*yclick <= 900)
        {
            //receive the click event
            setDown(true);

            if(e->type()==QEvent::MouseButtonPress)
            {
                emit pressed();
            }

            e->accept();
        }
        else
        {
            //propagate the click event to its parent
            if(e->type()!=QEvent::MouseMove)
            {
                e->ignore();
            }
            else
            {
                prog->mouseMoveEvent(e);
            }
        }
    } else {
        //if it's not a left click, just disregard the click
        e->accept();
    }
}

