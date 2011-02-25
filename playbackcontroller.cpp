#include "playbackcontroller.h"
#include <QString>
#include <QtGui>

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

    QGridLayout *temp = new QGridLayout(tempw);
    temp->setSpacing(0);
    temp->setMargin(0);

    //playback slider
    QHBoxLayout *playbackbox = new QHBoxLayout();
    playbackbox->setContentsMargins(15, 0, 15, 0);
    QSlider *playback = new QSlider(Qt::Horizontal);
    playback->setMaximum(10000);
    playback->setObjectName("bottomBarPlayback");
    playbackbox->addWidget(playback);

    //playback controls
    QGridLayout* controls = new QGridLayout();
    QHBoxLayout* left = new QHBoxLayout();
    QHBoxLayout* middle = new QHBoxLayout();
    QHBoxLayout* right = new QHBoxLayout();
    left->setSpacing(35);
    left->setMargin(0);
    left->setContentsMargins(40,0,0,0);
    middle->setSpacing(0);
    middle->setContentsMargins(35,0,35,0);
    middle->setMargin(0);
    right->setSpacing(10);
    right->setMargin(0);
    controls->addLayout(left, 0, 1, Qt::AlignRight | Qt::AlignVCenter);
    controls->addLayout(middle, 0, 3, Qt::AlignCenter | Qt::AlignVCenter);
    controls->addLayout(right, 0, 5, Qt::AlignLeft | Qt::AlignVCenter);
    controls->setSpacing(0);
    controls->setMargin(0);
    controls->setColumnStretch(0, 1);
    controls->setColumnStretch(1, 0);
    controls->setColumnStretch(2, 0);
    controls->setColumnStretch(3, 0);
    controls->setColumnStretch(4, 0);
    controls->setColumnStretch(5, 0);
    controls->setColumnStretch(6, 1);
    controls->setColumnMinimumWidth(2, 35);
    controls->setColumnMinimumWidth(4, 35);

    QPushButton *repeat = new QPushButton();
    repeat->setObjectName("bottomBarRepeat");
    repeat->setMaximumSize(57, 36);
    repeat->setMinimumSize(57, 36);
    //repeat->setFlat(true);
    QPushButton *shuffle = new QPushButton();
    repeat->setObjectName("bottomBarShuffle");
    shuffle->setMaximumSize(57, 36);
    shuffle->setMinimumSize(57, 36);
    //shuffle->setFlat(true);
    QPushButton *previous = new QPushButton();
    previous->setObjectName("bottomBarPrevious");
    previous->setMaximumSize(42, 39);
    previous->setMinimumSize(42, 39);
    //previous->setFlat(true);
    QPushButton *playpause = new QPushButton();
    playpause->setObjectName("bottomBarPlaypause");
    playpause->setMaximumSize(48, 59);
    playpause->setMinimumSize(48, 59);
    //playpause->setFlat(true);
    QPushButton *next = new QPushButton();
    next->setObjectName("bottomBarNext");
    next->setMaximumSize(42, 39);
    next->setMinimumSize(42, 39);
    //next->setFlat(true);
    QPushButton *mute = new QPushButton();
    mute->setObjectName("bottomBarMute");
    mute->setMaximumSize(57, 36);
    mute->setMinimumSize(57, 36);
    //mute->setFlat(true);
    QSlider *volumeslider = new QSlider(Qt::Horizontal);
    volumeslider->setObjectName("bottomBarVolumeslider");
    volumeslider->setFixedWidth(125);
    left->addWidget(repeat);
    left->addWidget(shuffle);
    middle->addWidget(previous);
    middle->addWidget(playpause);
    middle->addWidget(next);
    right->addWidget(mute);
    right->addWidget(volumeslider);

    temp->addLayout(controls, 1, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    temp->addLayout(playbackbox, 0, 0, Qt::AlignHCenter | Qt::AlignTop);
    temp->setRowMinimumHeight(2, 5);
    temp->setRowStretch(0, 0);

    //player.playFile("/home/vity/01-Metric-Help I'm Alive.mp3");
    connect(playpause, SIGNAL(clicked()), &player, SLOT(playControl()));
    connect(volumeslider, SIGNAL(valueChanged(int)), &player, SLOT(changeVolume(int)));
    connect(playback, SIGNAL(sliderMoved(int)), &player, SLOT(changePosition(int)));
    connect(mute, SIGNAL(clicked()), &player, SLOT(muteAudio()));
    connect(&player, SIGNAL(sliderChanged(int)), playback, SLOT(setValue(int)));
    connect(next, SIGNAL(clicked()), this, SIGNAL(nextFile()));
    connect(previous, SIGNAL(clicked()), this, SIGNAL(prevFile()));

    return tempw;
}

/*void PlaybackController::nextFile()
{
    emit nextFile();
}*/
