#ifndef PLAYBACKPROGRESS_H
#define PLAYBACKPROGRESS_H

#include <QFrame>
#include "utilities.h"
class PlaybackProgress : public QFrame
{
    Q_OBJECT
public:
    explicit PlaybackProgress(Utilities& utilities, QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void recalculateAngle(QPointF);
    void paintEvent(QPaintEvent *);
signals:
    void newAngle(int pieangle);

public slots:
    void setAngle(int angle);

private:
    Utilities& util;
    int pieangle;

signals:
    void mousePressEvent();
    void mouseReleaseEvent();

};

#endif // PLAYBACKPROGRESS_H
