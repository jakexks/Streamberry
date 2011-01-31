
#include "playlists.h"
#include <QtGui>
#include <QDebug>

#define TOPBARHEIGHT 32
#define BOTTOMBARHEIGHT 34

/*

 These changes to be made in the mainwindow file in the needed place

 #define TOPBARHEIGHT 36


    playlists* sidebar = new playlists();
    topbar = makeTopBar();
    leftbar = sidebar->makesidebar(expath);
    bottombar = makeBottomBar();
    rightside = makeRightSide();
 */
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

/*QWidget* playlists::makePlaylistsTitle()
{
  QWidget *tempw = new QWidget();
  tempw->setObjectName("PlaylistTitle");
  QString style = "QWidget#PlaylistTitleWidget { background-image: url(";
  style += expath;
  style += "images/bgPlaylistBarTop.png);}";
  tempw->setStyleSheet(style);
  QGridLayout *temp = new QGridLayout(tempw);
  return tempw;
}*/


QPushButton* Playlists::makePreviewBtn()
{
  QPushButton* temp = new QPushButton();
  temp->setObjectName("PreviewBtn");
  QString style = "background-image: url(";
  style += expath;
  style += "images/btnPreview.png); background-repeat: no-repeat; border:none;";
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
  QString style = "background-image: url(";
  style += expath;
  style += "images/bgPlaylistBarTop.png); background-repeat: no-repeat; border:none;";
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
  qDebug() << "working one:";
  qDebug() << style;
  QGridLayout *temp = new QGridLayout(tempw);
  //put widgets into layout here
  return tempw;
}


