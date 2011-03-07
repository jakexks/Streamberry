#ifndef GUITOPBARCONTROLLER_H
#define GUITOPBARCONTROLLER_H

#include "utilities.h"
#include <sbsearchbar.h>
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
    SBSearchBar* getSearchbar();
public slots:
    void musicVideo();
private:
    QString expath;
    Utilities& util;
    QWidget* widget;
    QPushButton *music;
    QPushButton *videos;
    SBSearchBar *search;
};

#endif // GUITOPBARCONTROLLER_H
