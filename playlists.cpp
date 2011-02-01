
#include "playlists.h"
#include <QtGui>
#include <QDebug>

#define TOPBARHEIGHT 32
#define BOTTOMBARHEIGHT 34

void Playlists::setPath(char* path)
{
    expath = path;
}

Playlists::Playlists()
{
   sidewidget = new QWidget();
}

QWidget* Playlists::makeSidebar(QString path)
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
    sidebarlayout = new QGridLayout(sidewidget)                    ;
    sidebarlayout->setRowMinimumHeight(0, TOPBARHEIGHT);
    sidebarlayout->setRowStretch(0, 0);
    sidebarlayout->setRowStretch(1, 2);
    sidebarlayout->setRowStretch(2, 0);
    sidebarlayout->setRowStretch(3, 0);
    sidebarlayout->setRowMinimumHeight(2, BOTTOMBARHEIGHT);
    sidebarlayout->setRowMinimumHeight(3, 140);
    sidebarlayout->setColumnMinimumWidth(0, 140);
    sidebarlayout->setMargin(0);
    sidebarlayout->setSpacing(0);

    playlistbtn = makePlaylistBtn();
    playlistbar = makePlaylistBar();
    previewbar = makePlaylistBar();
    previewbtn = makePreviewBtn();

    sidebarlayout->addWidget(playlistbtn, 0, 0);
    sidebarlayout->addWidget(playlistbar, 1, 0);
    sidebarlayout->addWidget(previewbtn, 2, 0);
    sidebarlayout->addWidget(previewbar, 3, 0);
    return sidewidget;
}



QPushButton* Playlists::makePreviewBtn()
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

QPushButton* Playlists::makePlaylistBtn()
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

QWidget* Playlists::makePlaylistBar()
{
  QWidget *tempw = new QWidget();
  tempw->setObjectName("PlaylistBarWidget");
  QString style = "QWidget#PlaylistBarWidget { background-image: url(";
  style += expath;
  style += "images/bgSideBar.png); background-repeat: repeat-y }";
  tempw->setStyleSheet(style);
  QGridLayout *temp = new QGridLayout(tempw);
  return tempw;
}


