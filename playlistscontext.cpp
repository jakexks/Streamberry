#include "playlistscontext.h"
#include <QMenu>
#include "playlist.h"
#include "utilities.h"
#include "librarycontroller.h"
#include "sidebarcontroller.h"
#include <QtGui>

//PlaylistMenu::PlaylistMenu(bool smart, Database &datab)
PlaylistMenu::PlaylistMenu(bool smart)
{
  //db = datab;
  if(smart == false)
  {
    QAction* menuitems[5];
    menuitems[0] = this->addAction("Play");
    menuitems[1] = this->addSeparator();
    menuitems[2] = this->addAction("View");
    menuitems[3] = this->addAction("Rename");
    menuitems[4] = this->addAction("Delete");

    QObject::connect(menuitems[0], SIGNAL(triggered()), this, SLOT(NormalPlay()));
    QObject::connect(menuitems[2], SIGNAL(triggered()), this, SLOT(NormalView()));
    QObject::connect(menuitems[3], SIGNAL(triggered()), this, SLOT(NormalRename()));
    QObject::connect(menuitems[4], SIGNAL(triggered()), this, SLOT(NormalDelete()));
  }
  else
  {
    QAction* menuitems[6];
    menuitems[0] = this->addAction("Play");
    menuitems[1] = this->addSeparator();
    menuitems[2] = this->addAction("View");
    menuitems[3] = this->addAction("Rename");
    menuitems[4] = this->addAction("Edit");
    menuitems[5] = this->addAction("Delete");

    QObject::connect(menuitems[0], SIGNAL(triggered()), this, SLOT(SmartPlay()));
    QObject::connect(menuitems[2], SIGNAL(triggered()), this, SLOT(SmartView()));
    QObject::connect(menuitems[3], SIGNAL(triggered()), this, SLOT(SmartRename()));
    QObject::connect(menuitems[4], SIGNAL(triggered()), this, SLOT(SmartEdit()));
    QObject::connect(menuitems[5], SIGNAL(triggered()), this, SLOT(SmartDelete()));
  }
}

void PlaylistMenu::playlistrightclicked(Playlist* passplaylist, LibraryController* passlib)
{
  PL = passplaylist;
  LibCont = passlib;
  this->exec(QCursor::pos());
}

void PlaylistMenu::SmartPlay()
{
  qDebug() << PL->getPlaylistName();
  qDebug() << "Play Now!";
}

void PlaylistMenu::SmartView()
{
  qDebug() << "click";
}

void PlaylistMenu::SmartRename()
{
  qDebug() << "click";
}

void PlaylistMenu::SmartEdit()
{
  qDebug() << "click";
}

void PlaylistMenu::SmartDelete()
{
  PL->deletePlaylist();
}


void PlaylistMenu::NormalPlay()
{
  qDebug() << PL->getPlaylistName();
  qDebug() << "Play Now!";
}

void PlaylistMenu::NormalView()
{
  QList<QSqlRecord>* alltracks = PL->getAllTracks();
  //qDebug() << *alltracks;
  LibCont->fillData(alltracks);
}

void PlaylistMenu::NormalRename()
{
  qDebug() << "click";
}

void PlaylistMenu::NormalDelete()
{
  PL->deletePlaylist();
}


