
#include "playlists.h"
#include <QtGui>
#include <QDebug>

#define TOPBARHEIGHT 32
#define BOTTOMBARHEIGHT 34

void playlists::setPath(char* path)
{
    expath = path;
}

playlists::playlists()
{
   SideWidget = new QWidget();
}

QWidget* playlists::makesidebar(QString path)
{
    expath = path;
    SideBarLayout = new QGridLayout(SideWidget)                    ;
    SideBarLayout->setRowMinimumHeight(0, TOPBARHEIGHT);
    SideBarLayout->setRowStretch(0, 0);
    SideBarLayout->setRowStretch(1, 2);
    SideBarLayout->setRowStretch(2, 0);
    SideBarLayout->setRowStretch(3, 0);
    SideBarLayout->setRowMinimumHeight(2, BOTTOMBARHEIGHT);
    SideBarLayout->setRowMinimumHeight(3, 140);
    SideBarLayout->setColumnMinimumWidth(0, 140);
    SideBarLayout->setMargin(0);
    SideBarLayout->setSpacing(0);

    PlaylistBtn = makePlaylistBtn();
    PlaylistBar = makePlaylistBar();
    PreviewBar = makePlaylistBar();
    PreviewBtn = makePreviewBtn();

    SideBarLayout->addWidget(PlaylistBtn, 0, 0);
    SideBarLayout->addWidget(PlaylistBar, 1, 0);
    SideBarLayout->addWidget(PreviewBtn, 2, 0);
    SideBarLayout->addWidget(PreviewBar, 3, 0);

    return SideWidget;
}

QPushButton* playlists::makePreviewBtn()
{
  QPushButton* temp = new QPushButton();
  temp->setObjectName("PreviewBtn");
  QString style = "QPushButton { background-image: url(";
  style += expath;
  style += "images/btnPreview.png); background-repeat: no-repeat; border:none;} QPushButton:pressed {";
  style += "background-image: url(";
  style += expath;
  style += "images/btnPreviewClicked.png); background-repeat: no-repeat; border:none;} QPushButton:hover:!pressed {";
  style += "background-image: url(";
  style += expath;
  style += "images/btnPreviewGlow.png); background-repeat: no-repeat; border:none;}";
  temp->setStyleSheet(style);
  temp->setMaximumSize(190, 43);
  temp->setMinimumSize(190, 43);
  temp->setFlat(true);
  return temp;
}

QPushButton* playlists::makePlaylistBtn()
{
  QPushButton* temp = new QPushButton();
  temp->setObjectName("PlaylistBtn");
  QString style = "QPushButton { background-image: url(";
  style += expath;
  style += "images/bgPlaylistBarTop.png); background-repeat: no-repeat; border:none;} QPushButton:pressed {";
  style += "background-image: url(";
  style += expath;
  style += "images/bgPlaylistBarTopClicked.png); background-repeat: no-repeat; border:none;} QPushButton:hover:!pressed {";
  style += "background-image: url(";
  style += expath;
  style += "images/bgPlaylistBarTopGlow.png); background-repeat: no-repeat; border:none;}";
  temp->setStyleSheet(style);
  temp->setMaximumSize(190, 40);
  temp->setMinimumSize(190, 40);
  temp->setFlat(true);  
  return temp;
}

QWidget* playlists::makePlaylistBar()
{
  QWidget *tempw = new QWidget();
  tempw->setObjectName("PlaylistBarWidget");
  QString style = "QWidget#PlaylistBarWidget { background-image: url(";
  style += expath;
  style += "images/bgSideBar.png); background-repeat: repeat-y }";
  tempw->setStyleSheet(style);
  //qDebug() << style;
  QGridLayout *temp = new QGridLayout(tempw);
  //put widgets into layout here
  return tempw;
}


