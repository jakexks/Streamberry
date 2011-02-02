#ifndef GUITOPBARCONTROLLER_H
#define GUITOPBARCONTROLLER_H

#include "utilities.h"
#include <QWidget>
#include <QString>

class TopbarController
{
public:
    TopbarController(Utilities &utilities);
    QWidget* makeWidget();
    QWidget* getWidget();
private:
    QString expath;
    Utilities& util;
    QWidget* widget;
};

#endif // GUITOPBARCONTROLLER_H
