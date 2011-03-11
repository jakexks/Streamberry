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
    enum MusicVideoCheckState { MusicVideoNotChecked = 2, MusicChecked = 0, VideoChecked = 1 };
signals:
    void musicVideoCheckStateChanged(int);
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
    SBSearchBar *search;
};

#endif // GUITOPBARCONTROLLER_H
