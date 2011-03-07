#ifndef GUITOPBARCONTROLLER_H
#define GUITOPBARCONTROLLER_H

#include "utilities.h"
#include <QWidget>
#include <QString>
#include <QObject>
#include <QPushButton>

class TopbarController : public QObject
{
    Q_OBJECT
public:
    TopbarController(Utilities &utilities);
    QWidget* makeWidget();
    QWidget* getWidget();
public slots:
    void musicButtonControl();
    void videosButtonControl();
private:
    QString expath;
    Utilities& util;
    QWidget* widget;
    QPushButton *music;
    QPushButton *videos;
    int musicState;
    int videosState;
};

#endif // GUITOPBARCONTROLLER_H
