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
    void goBackPlease();
    void goForwardPlease();
public slots:
    void musicButtonControl();
    void videosButtonControl();
    void backClicked();
    void forwardClicked();
    void setSearchText(QString text);
private:
    QString expath;
    Utilities& util;
    QWidget* widget;
    QPushButton *music;
    QPushButton *videos;
    QPushButton *back;
    QPushButton *forward;
    int musicState;
    int videosState;
    SBSearchBar *search;
};

#endif // GUITOPBARCONTROLLER_H
