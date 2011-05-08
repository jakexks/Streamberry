#ifndef PLAYBACKPROGRESS_H
#define PLAYBACKPROGRESS_H

#include <QFrame>
#include "utilities.h"

//the playback progress bar in the playback controls
class PlaybackProgress : public QFrame
{
    Q_OBJECT
public:
    explicit PlaybackProgress(Utilities& utilities, QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    //calculate angle from where a mouse is pressed
    void recalculateAngle(QPointF);
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
signals:
    //angle needs to be changed changed
    void newAngle(int pieangle);

public slots:
    //change angle
    void setAngle(int angle);

private:
    Utilities& util;
    //how the angle is stored
    int pieangle;
    //whether the mouse is over the object
    bool isentered;
    //whether the mouse is dragging
    bool isdragging;
    //the image of the handle
    QImage handleimg;

signals:
    void mousePressEvent();
    void mouseReleaseEvent();

};

#endif // PLAYBACKPROGRESS_H
