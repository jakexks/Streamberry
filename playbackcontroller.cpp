#include "playbackcontroller.h"
#include <QString>
#include <QtGui>
#include "playbackprogress.h"
#include "playbackbutton.h"

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

    QGridLayout *songInfoArea = new QGridLayout();
    //playback controls
//    QGridLayout *controlsContainer = new QGridLayout();
//    QGridLayout* controls = new QGridLayout();
//    QHBoxLayout* left = new QHBoxLayout();
//    QHBoxLayout* middle = new QHBoxLayout();
//    QHBoxLayout* right = new QHBoxLayout();

//    controlsContainer->setSpacing(0);
//    controlsContainer->setMargin(0);
    songInfoArea->setSpacing(0);
    songInfoArea->setMargin(10);
//    left->setSpacing(35);
//    left->setMargin(0);
//    left->setContentsMargins(40,0,0,0);
//    middle->setSpacing(0);
//    middle->setContentsMargins(37,0,37,0);
//    middle->setMargin(0);
//    right->setSpacing(10);
//    right->setMargin(0);
//    controls->addLayout(left, 0, 1, Qt::AlignRight | Qt::AlignVCenter);
//    controls->addLayout(middle, 0, 3, Qt::AlignCenter | Qt::AlignVCenter);
//    controls->addLayout(right, 0, 5, Qt::AlignLeft | Qt::AlignVCenter);
//    controls->setSpacing(0);
//    controls->setMargin(0);
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
//    controls->setColumnMinimumWidth(2, 35);
//    controls->setColumnMinimumWidth(4, 35);

    QPushButton *repeat = new QPushButton();
    repeat->setObjectName("bottomBarRepeat");
    repeat->setMaximumSize(40, 38);
    repeat->setMinimumSize(40, 38);
    repeat->setStyleSheet(util.getStylesheet());
    repeat->setFlat(true);

    QPushButton *shuffle = new QPushButton();
    shuffle->setObjectName("bottomBarShuffle");
    shuffle->setMaximumSize(40, 38);
    shuffle->setMinimumSize(40, 38);
    shuffle->setStyleSheet(util.getStylesheet());
    shuffle->setFlat(true);
    QPushButton *previous = new QPushButton();
    previous->setObjectName("bottomBarPrevious");
    previous->setMaximumSize(45, 37);
    previous->setMinimumSize(45, 37);
    previous->setStyleSheet(util.getStylesheet());
    previous->setFlat(true);

    PlaybackProgress::PlaybackProgress *progressbar = new PlaybackProgress(util);
    PlaybackButton::PlaybackButton *playbutton = new PlaybackButton(progressbar);
    QPushButton *next = new QPushButton();
    next->setObjectName("bottomBarNext");
    next->setMaximumSize(45, 37);
    next->setMinimumSize(45, 37);
    next->setStyleSheet(util.getStylesheet());
    next->setFlat(true);
    QPushButton *mute = new QPushButton();
    mute->setObjectName("bottomBarMute");
    mute->setMaximumSize(40, 38);
    mute->setMinimumSize(40, 38);
    //works without the setStyleSheet
    mute->setFlat(true);

    QSlider *volumeslider = new QSlider(Qt::Horizontal);
    volumeslider->setObjectName("bottomBarVolumeslider");
    volumeslider->setFixedWidth(110);
    volumeslider->setValue(50);

    QLabel *songInfoLine = new QLabel();
    songInfoLine->setObjectName("bottomBarSongInfoLine");
    songInfoLine->setMinimumWidth(1);
    songInfoLine->setMaximumWidth(1);
    songInfoLine->setMinimumHeight(64);
    songInfoLine->setStyleSheet(util.getStylesheet());

    songInfoArea->addWidget(songInfoLine);
//    left->addWidget(shuffle);
//    left->addWidget(repeat);
//    middle->addWidget(previous);
//    middle->addWidget(progressBar);
//    middle->addWidget(next);
//    right->addWidget(mute);
//    right->addWidget(volumeslider);

    temp->addLayout(songInfoArea, 0, 0);
    temp->setColumnMinimumWidth(0, 222);
    temp->setColumnMinimumWidth(2, 70);
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
//    temp->addLayout(controlsContainer,1,1);
//    controlsContainer->addLayout(controls, 1, 0, Qt::AlignHCenter | Qt::AlignVCenter);
//    temp->setRowMinimumHeight(2, 5);
//    temp->setRowStretch(0, 0);

    //player.playFile("/home/vity/01-Metric-Help I'm Alive.mp3");
    connect(playbutton, SIGNAL(clicked()), &player, SLOT(playControl()));
    connect(volumeslider, SIGNAL(valueChanged(int)), &player, SLOT(changeVolume(int)));
//    connect(dial, SIGNAL(progressBar.mousePressEvent(int)), &player, SLOT(changePosition(progressBar.mouseReleaseEvent)));
    connect(mute, SIGNAL(clicked()), &player, SLOT(muteAudio()));
//    connect(&player, SIGNAL(sliderChanged(int)), playback, SLOT(setValue(int)));
    connect(next, SIGNAL(clicked()), this, SIGNAL(nextFile()));
    connect(previous, SIGNAL(clicked()), this, SIGNAL(prevFile()));

    return tempw;
}

/*void PlaybackController::nextFile()
{
    emit nextFile();
}*/
