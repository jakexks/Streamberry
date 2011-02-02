#include "playbackcontroller.h"
#include <QString>
#include <QtGui>

PlaybackController::PlaybackController(Utilities &utilities) : util(utilities)
{
    widget = makeWidget();
}

QWidget* PlaybackController::getWidget()
{
    return widget;
}

QWidget* PlaybackController::makeWidget()
{
    QString expath(util.getExecutePath());
    QWidget *tempw = new QWidget();
    tempw->setObjectName("bottomBarWidget");
    QString style = "QWidget#bottomBarWidget { background-image: url(";
    style += expath;
    style += "images/bgBottomBar.png); background-repeat: repeat-x }";
    tempw->setStyleSheet(style);

    QGridLayout *temp = new QGridLayout(tempw);
    temp->setSpacing(0);
    temp->setMargin(0);

    //playback slider
    QHBoxLayout *playbackbox = new QHBoxLayout();
    playbackbox->setContentsMargins(15, 0, 15, 0);
    QSlider *playback = new QSlider(Qt::Horizontal);
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
    style = "background-image: url(";
    style += expath;
    style += "images/btnRepeatUp.png); background-repeat: no-repeat; border:none;";
    repeat->setStyleSheet(style);
    repeat->setMaximumSize(57, 36);
    repeat->setMinimumSize(57, 36);
    repeat->setFlat(true);
    QPushButton *shuffle = new QPushButton();
    style = "background-image: url(";
    style += expath;
    style += "images/btnShuffleUp.png); background-repeat: no-repeat; border:none;";
    shuffle->setStyleSheet(style);
    shuffle->setMaximumSize(57, 36);
    shuffle->setMinimumSize(57, 36);
    shuffle->setFlat(true);
    QPushButton *previous = new QPushButton();
    style = "background-image: url(";
    style += expath;
    style += "images/btnPreviousUp.png); background-repeat: no-repeat; border:none;";
    previous->setStyleSheet(style);
    previous->setMaximumSize(42, 39);
    previous->setMinimumSize(42, 39);
    previous->setFlat(true);
    QPushButton *playpause = new QPushButton();
    style = "background-image: url(";
    style += expath;
    style += "images/btnPlayUp.png); background-repeat: no-repeat; border:none;";
    playpause->setStyleSheet(style);
    playpause->setMaximumSize(48, 59);
    playpause->setMinimumSize(48, 59);
    playpause->setFlat(true);
    QPushButton *next = new QPushButton();
    style = "background-image: url(";
    style += expath;
    style += "images/btnNextUp.png); background-repeat: no-repeat; border:none;";
    next->setStyleSheet(style);
    next->setMaximumSize(42, 39);
    next->setMinimumSize(42, 39);
    next->setFlat(true);
    QPushButton *mute = new QPushButton();
    style = "background-image: url(";
    style += expath;
    style += "images/btnMuteUp.png); background-repeat: no-repeat; border:none;";
    mute->setStyleSheet(style);
    mute->setMaximumSize(57, 36);
    mute->setMinimumSize(57, 36);
    mute->setFlat(true);
    QSlider *volumeslider = new QSlider(Qt::Horizontal);
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
    return tempw;
}
