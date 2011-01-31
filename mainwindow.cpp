#include "mainwindow.h"
#include <QtGui>
#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include "playlists.h"
#include "sbsearchbar.h"

//changed from 33 to match the flat better (jim) (can be changed back if we like this version more)
#define TOPBARHEIGHT 36
#define BOTTOMBARHEIGHT 90

MainWindow::MainWindow(char* path, QWidget *parent)
    : QMainWindow(parent)
{
    expath = QString(path);
    expath.resize(expath.lastIndexOf('/'));
    expath += "/";
    menubar = createMenuBar();

    this->setWindowTitle("Streamberry");
    this->setMenuBar(menubar);
    resize(850, 600);
    centralwidget = new QWidget();
    mainlayout = new QGridLayout(centralwidget);
    mainlayout->setMargin(0);
    mainlayout->setSpacing(0);

    mainlayout->setRowMinimumHeight(0, TOPBARHEIGHT);
    mainlayout->setRowStretch(0, 0);
    mainlayout->setRowStretch(1, 1);
    mainlayout->setRowMinimumHeight(2, BOTTOMBARHEIGHT);
    mainlayout->setRowStretch(2, 0);
    mainlayout->setColumnMinimumWidth(0, 190);
    mainlayout->setColumnStretch(0, 0);
    mainlayout->setColumnStretch(1, 1);

    Playlists* sidebar = new Playlists();
    topbar = makeTopBar();
    leftbar = sidebar->makeSidebar(expath);
    bottombar = makeBottomBar();
    rightside = makeRightSide();

    mainlayout->addWidget(topbar, 0, 1);
    mainlayout->addWidget(leftbar, 0, 0, 2, 1);
    mainlayout->addWidget(rightside, 1, 1);
    mainlayout->addWidget(bottombar, 2, 0, 1, 2);

    setCentralWidget(centralwidget);
}

QWidget* MainWindow::makeTopBar() {
    QWidget *tempw = new QWidget();
    tempw->setObjectName("topBarWidget");
    QString style = "QWidget#topBarWidget { background-image: url(";
    style += expath;
    style += "images/bgTopBar.png); background-repeat: repeat-x }";
    tempw->setStyleSheet(style);

    //Grids to hold objects in
    QGridLayout *outer = new QGridLayout(tempw);
    outer->setMargin(0);
    outer->setSpacing(0);
    QGridLayout *innerleft = new QGridLayout();
    innerleft->setMargin(0);
    innerleft->setSpacing(0);
    innerleft->setContentsMargins(0,2,0,0);
    QGridLayout *innermiddle = new QGridLayout();
    innermiddle->setMargin(0);
    innermiddle->setSpacing(0);
    innermiddle->setContentsMargins(0,2,0,0);
    QGridLayout *innerright = new QGridLayout();
    innerright->setMargin(0);
    innerright->setSpacing(0);
    //Buttons for menu
    QPushButton *back = new QPushButton();
    QPushButton *forward = new QPushButton();
    QPushButton *music = new QPushButton();
    QPushButton *videos = new QPushButton();
    SBSearchBar *search = new SBSearchBar(expath);
    //Split it into three sections
    outer->addLayout(innerleft, 1, 0);
    outer->addLayout(innermiddle, 0, 1, 3, 1);
    outer->addLayout(innerright, 1, 2);
    //set which rows and columns in outer layout are stretchy
    outer->setRowStretch(0, 1);
    outer->setRowStretch(1, 0);
    outer->setRowStretch(2, 1);
    outer->setColumnStretch(0, 0);
    outer->setColumnStretch(1, 1);
    outer->setColumnStretch(2, 0);
    //Set which are stretchy in inner layouts
    innermiddle->setColumnStretch(0, 1);
    innermiddle->setColumnStretch(1, 0);
    innermiddle->setColumnStretch(2, 0);
    innermiddle->setColumnStretch(3, 1);
    //Set measurements of spacer columns
    innerleft->setColumnMinimumWidth(0, 15);
    innerleft->setColumnMinimumWidth(2, 5);
    innermiddle->setColumnMinimumWidth(0, 40);
    innermiddle->setColumnMinimumWidth(3, 40);
    innerright->setColumnMinimumWidth(1, 15);
    //style the buttons
    style = "background-image: url(";
    style += expath;
    style += "images/btnBackUp.png); background-repeat: no-repeat; border:none;";
    back->setStyleSheet(style);
    back->setMaximumSize(35, 29);
    back->setMinimumSize(35, 29);
    back->setFlat(true);
    style = "background-image: url(";
    style += expath;
    style += "images/btnForwardUp.png); background-repeat: no-repeat; border:none;";
    forward->setStyleSheet(style);
    forward->setMaximumSize(35, 29);
    forward->setMinimumSize(35, 29);
    forward->setFlat(true);
    style = "background-image: url(";
    style += expath;
    style += "images/btnMusicUp.png); background-repeat: no-repeat; border:none;";
    music->setStyleSheet(style);
    music->setMaximumSize(106, 33);
    music->setMinimumSize(106, 33);
    music->setFlat(true);
    style = "background-image: url(";
    style += expath;
    style += "images/btnVideosUp.png); background-repeat: no-repeat; border:none;";
    videos->setStyleSheet(style);
    videos->setMaximumSize(114, 33);
    videos->setMinimumSize(114, 33);
    videos->setFlat(true);
    //Add buttons to each section
    innerleft->addWidget(back, 0, 1);
    innerleft->addWidget(forward, 0, 3);
    innermiddle->addWidget(music, 0, 1);
    innermiddle->addWidget(videos, 0, 2);
    innerright->addWidget(search, 0, 0);
    return tempw;
}

QWidget* MainWindow::makeLeftBar() {
    QWidget *tempw = new QWidget();
    tempw->setObjectName("leftBarWidget");
    tempw->setStyleSheet("QWidget#leftBarWidget { background-color: #D9DFE7 }");

    QGridLayout *temp = new QGridLayout(tempw);
    //put widgets into layout here
    return tempw;
}

QWidget* MainWindow::makeBottomBar() {
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

QWidget* MainWindow::makeRightSide() {
    QWidget *tempw = new QWidget();
    tempw->setObjectName("rightSideWidget");
    tempw->setStyleSheet("QWidget#rightSideWidget { background-color: #FFFFFF }");

    QGridLayout *temp = new QGridLayout(tempw);
    temp->setMargin(0);

    QTableWidget *tableWidget = new QTableWidget(12, 3);
    tableWidget->setObjectName("libraryTableWidget");
    tableWidget->setStyleSheet("QTableWidget#libraryTableWidget { border:none; }");
    tableWidget->setRowCount(10);
    tableWidget->setColumnCount(5);

    QTableWidgetItem *newItem = new QTableWidgetItem(expath);
    tableWidget->setItem(5, 2, newItem);
    temp->addWidget(tableWidget);

    return tempw;
}

QMenuBar* MainWindow::createMenuBar()
{
    QMenuBar* menu = new QMenuBar();
    menu->setNativeMenuBar(true);
    QMenu* file = menu->addMenu("File");
    QMenu* view = menu->addMenu("View");
    QMenu* controls = menu->addMenu("View");
    QMenu* window = menu->addMenu("Window");
    QMenu* help = menu->addMenu("Help");
    return menu;
}

MainWindow::~MainWindow()
{
}
