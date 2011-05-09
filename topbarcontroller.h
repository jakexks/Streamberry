#ifndef GUITOPBARCONTROLLER_H
#define GUITOPBARCONTROLLER_H

#include "utilities.h"
#include <sbsearchbar.h>
#include <QWidget>
#include <QString>
#include <QObject>
#include <QPushButton>

//Controls the top bar in the GUI
class TopbarController : public QObject
{
    Q_OBJECT
public:
    TopbarController(Utilities &utilities);
    //make widget
    QWidget* makeWidget();
    //return widget
    QWidget* getWidget();
    //put search bar in place
    SBSearchBar* getSearchbar();
    enum MusicVideoCheckState { MusicVideoNotChecked = 2, MusicChecked = 0, VideoChecked = 1 };
signals:
    void musicVideoCheckStateChanged(int);
    //tell library controller to go back
    void goBackPlease();
    //tell library controller to go forward
    void goForwardPlease();
public slots:
    //music button pressed
    void musicButtonControl();
    //video button pressed
    void videosButtonControl();
    //back button pressed
    void backClicked();
    //forward button pressed
    void forwardClicked();
    //set the search text
    void setSearchText(QString text);
private:
    QString expath;
    Utilities& util;
    QWidget* widget;
    QPushButton *music;
    QPushButton *videos;
    QPushButton *back;
    QPushButton *forward;
    //booleans for the states of the music/video button
    int musicState;
    int videosState;
    //creates the search bar widget
    SBSearchBar *search;
};

#endif // GUITOPBARCONTROLLER_H
