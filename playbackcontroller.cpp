#include "playbackcontroller.h"
#include <QString>
#include <QtGui>
#include "playbackprogress.h"
#include "playbackbutton.h"
#include "songinfo.h"
#include <QPalette>
#include "volumeslider.h"

#define BUTTON_DISTANCE 25
#define PLAYBACK_DISTANCE 50

PlaybackController::PlaybackController(Utilities &utilities, Player &p) : util(utilities), player(p)
{
    widget = makeWidget();
}

QWidget* PlaybackController::getWidget()
{
    return widget;
}

int PlaybackController::getvolpos()
{
  return volumeslider->getlastpos();
}

void PlaybackController::setvolumepos(int j)
{
  volumeslider->setslidepos(j);
}

QWidget* PlaybackController::makeWidget()
{
    QWidget *tempw = new QWidget();
    tempw->setObjectName("bottomBarWidget");
    tempw->setMinimumHeight(96);
    tempw->setMaximumHeight(96);
    tempw->setStyleSheet(util.getStylesheet());
    QGridLayout *temp = new QGridLayout(tempw);
    temp->setSpacing(0);
    temp->setMargin(0);
    temp->setColumnStretch(0, 0);
    temp->setColumnStretch(1, 1);
    temp->setColumnStretch(2, 0);
    temp->setColumnStretch(3, 0);
    temp->setColumnStretch(4, 0);
    temp->setColumnStretch(5, 0);
    temp->setColumnStretch(6, 0);
    temp->setColumnStretch(7, 0);
    temp->setColumnStretch(8, 0);
    temp->setColumnStretch(9, 0);
    temp->setColumnStretch(10, 0);
    temp->setColumnStretch(11, 0);
    temp->setColumnStretch(12, 0);
    temp->setColumnStretch(13, 0);
    temp->setColumnStretch(14, 1);

    repeat = new QPushButton();
    repeat->setObjectName("bottomBarRepeat");
    repeat->setMaximumSize(40, 38);
    repeat->setMinimumSize(40, 38);
    repeat->setFlat(true);
    repeat->setCheckable(true);
    repeat->setStyleSheet(util.getStylesheet());

    shuffle = new QPushButton();
    shuffle->setObjectName("bottomBarShuffle");
    shuffle->setMaximumSize(40, 38);
    shuffle->setMinimumSize(40, 38);
    shuffle->setFlat(true);
    shuffle->setCheckable(true);
    shuffle->setStyleSheet(util.getStylesheet());

    QPushButton *previous = new QPushButton();
    previous->setObjectName("bottomBarPrevious");
    previous->setMaximumSize(45, 37);
    previous->setMinimumSize(45, 37);
    previous->setStyleSheet(util.getStylesheet());
    previous->setFlat(true);

    PlaybackProgress *progressbar = new PlaybackProgress(util);
    PlaybackButton *playbutton = new PlaybackButton(util, progressbar);
    QPushButton *next = new QPushButton();
    next->setObjectName("bottomBarNext");
    next->setMaximumSize(45, 37);
    next->setMinimumSize(45, 37);
    next->setStyleSheet(util.getStylesheet());
    next->setFlat(true);
    mute = new QPushButton();
    mute->setCheckable(true);
    mute->setObjectName("bottomBarMute");
    mute->setMaximumSize(40, 38);
    mute->setMinimumSize(40, 38);
    //works without the setStyleSheet
    mute->setStyle(new QCleanlooksStyle);
    mute->setFlat(true);

    songinfoarea = new SongInfo(util);
    volumeslider = new VolumeSlider(util);
    volumeslider->setFixedWidth(110);

    temp->addWidget(songinfoarea->getWidget(), 0, 0);
    //makes the play button be in the middle, and not the widgets
    temp->setColumnMinimumWidth(1, 30);
    //temp->setColumnMinimumWidth(2, 70);
    temp->addWidget(shuffle, 0, 3);
    temp->setColumnMinimumWidth(4, BUTTON_DISTANCE);
    temp->addWidget(repeat, 0, 5);
    temp->setColumnMinimumWidth(6, PLAYBACK_DISTANCE);
    temp->addWidget(previous, 0, 7);
    temp->addWidget(progressbar, 0, 8);
    temp->addWidget(next, 0, 9);
    temp->setColumnMinimumWidth(10, PLAYBACK_DISTANCE);
    temp->addWidget(mute, 0, 11);
    temp->setColumnMinimumWidth(12, BUTTON_DISTANCE);
    temp->addWidget(volumeslider, 0, 13);
    temp->setColumnMinimumWidth(14, 30);

    //5760 is the highest
    connect(playbutton, SIGNAL(clicked()), &player, SLOT(playControl()));
    connect(volumeslider, SIGNAL(valueChanged(int)), &player, SLOT(changeVolume(int)));
  //  connect(volumeslider, SIGNAL(sliderPressed()), &player, SLOT(updateLastPos()));
    connect(volumeslider, SIGNAL(valueChanged(int)), this, SLOT(muteSlider()));
    connect(&player, SIGNAL(paused()), playbutton, SLOT(changeNamePause()));
    connect(&player, SIGNAL(play()), playbutton, SLOT(changeNamePlay()));
    connect(&player, SIGNAL(getNextFile()), playbutton, SLOT(changeNamePlay()));

//    connect(dial, SIGNAL(progressBar.mousePressEvent(int)), &player, SLOT(changePosition(progressBar.mouseReleaseEvent)));
    connect(mute, SIGNAL(clicked()), volumeslider, SLOT(muteVolSlider()));
    connect(&player, SIGNAL(sliderChanged(int)), progressbar, SLOT(setAngle(int)));
    connect(progressbar, SIGNAL(newAngle(int)), &player, SLOT(changePosition(int)));
    connect(next, SIGNAL(clicked()), this, SIGNAL(nextFile()));
    connect(previous, SIGNAL(clicked()), this, SIGNAL(prevFile()));
    connect(&player, SIGNAL(getNextFile()), next, SLOT(click()));
    connect(shuffle, SIGNAL(clicked()), this, SLOT(shufflePress()));
    connect(repeat, SIGNAL(clicked()), this, SLOT(repeatPress()));

    return tempw;
}

SongInfo* PlaybackController::getSongInfo()
{
    return songinfoarea;
}


void PlaybackController::muteSlider()
{
 if(volumeslider->sliderPosition()==0)
 {
   mute->setChecked(true);
 }
 else mute->setChecked(false);

}

void PlaybackController::shufflePress()
{
    if(shufflePressed==true)
    {
        shufflePressed=false;
        shuffle->setChecked(false);
        shuffle->setStyleSheet(util.getStylesheet());
        emit sPress();

    }
    else
    {
        shufflePressed=true;
        shuffle->setChecked(true);
        shuffle->setStyleSheet(util.getStylesheet());
        emit sPress();
    }

}

void PlaybackController::repeatPress()
{
    if(repeatOnePressed==true && repeatAllPressed==true)
    {
        repeatOnePressed=false;
        repeatAllPressed=false;
        repeat->setObjectName("bottomBarRepeat");
        repeat->setChecked(false);
        repeat->setStyleSheet(util.getStylesheet());
        emit rPress(false, false);
        return;
    }
    if(repeatOnePressed==true && repeatAllPressed==false)
    {
        repeatAllPressed=true;
        repeatOnePressed=true;
        repeat->setObjectName("bottomBarRepeat");
        repeat->setChecked(true);
        repeat->setStyleSheet(util.getStylesheet());
        emit rPress(true, true);
    }
    if(repeatOnePressed==false && repeatAllPressed==false)
    {
        repeatOnePressed=true;
        repeatAllPressed=false;
        repeat->setObjectName("bottomBarRepeatOne");
        repeat->setChecked(true);
        repeat->setStyleSheet(util.getStylesheet());
        emit rPress(true, false);
    }
}

/*void PlaybackController::nextFile()
{
    emit nextFile();
}*/
