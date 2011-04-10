#include "playlistscontext.h"
#include <QMenu>
#include "playlist.h"
#include "utilities.h"
#include "librarycontroller.h"
#include "sidebarcontroller.h"
#include <QtGui>
#include <playlistdialogue.h>
#include <plnormalnew.h>
#include <plsmartedit.h>
#include <plsmartnew.h>

//PlaylistMenu::PlaylistMenu(bool smart, Database &datab)
PlaylistMenu::PlaylistMenu(bool smart, Utilities *util1, Database& datab) : db(datab)
{
  setup = 0;
  libcont = NULL;
  util = util1;
  if(smart == false)
  {

    QAction* menuitems[9];
    menuitems[0] = this->addAction("Play");
    menuitems[1] = this->addSeparator();
    menuitems[2] = this->addAction("View");
    menuitems[3] = this->addAction("Rename");
    menuitems[4] = this->addAction("Delete");
    //menuitems[6] = this->addSeparator();
    //menuitems[7] = this->addAction("New Playlist");
    //menuitems[8] = this->addAction("New Smart Playlist");

    QObject::connect(menuitems[0], SIGNAL(triggered()), this, SLOT(NormalPlay()));
    QObject::connect(menuitems[2], SIGNAL(triggered()), this, SLOT(NormalView()));
    QObject::connect(menuitems[3], SIGNAL(triggered()), this, SLOT(NormalRename()));
    QObject::connect(menuitems[4], SIGNAL(triggered()), this, SLOT(NormalDelete()));

    //QObject::connect(menuitems[7], SIGNAL(triggered()), this, SLOT(NewPlay()));
    //QObject::connect(menuitems[8], SIGNAL(triggered()), this, SLOT(NewSmartPlay()));

  }
  else
  {
    QAction* menuitems[9];
    menuitems[0] = this->addAction("Play");
    menuitems[1] = this->addSeparator();
    menuitems[2] = this->addAction("View");
    menuitems[3] = this->addAction("Rename");
    menuitems[4] = this->addAction("Edit");
    menuitems[5] = this->addAction("Delete");
    //menuitems[6] = this->addSeparator();
    //menuitems[7] = this->addAction("New Playlist");
    //menuitems[8] = this->addAction("New Smart Playlist");

    QObject::connect(menuitems[0], SIGNAL(triggered()), this, SLOT(SmartPlay()));
    QObject::connect(menuitems[2], SIGNAL(triggered()), this, SLOT(SmartView()));
    QObject::connect(menuitems[3], SIGNAL(triggered()), this, SLOT(SmartRename()));
    QObject::connect(menuitems[4], SIGNAL(triggered()), this, SLOT(SmartEdit()));
    QObject::connect(menuitems[5], SIGNAL(triggered()), this, SLOT(SmartDelete()));

    //QObject::connect(menuitems[7], SIGNAL(triggered()), this, SLOT(NewPlay()));
    //QObject::connect(menuitems[8], SIGNAL(triggered()), this, SLOT(NewSmartPlay()));

  }



}

void PlaylistMenu::playlistrightclicked(Playlist* passplaylist, LibraryController* passlib)
{
  PL = passplaylist;
  if(setup == 0)
  {
    libcont = passlib;
    //QObject::connect(this, SIGNAL(playthis(QList<QSqlRecord>*)), libcont, SLOT(displaythis(QList<QSqlRecord>*)));
    setup = 1;
  }
  this->exec(QCursor::pos());
}

void PlaylistMenu::SmartPlay()
{
  emit playplaylist(PL->getPlaylistName());
}

void PlaylistMenu::SmartView()
{
  //QList<QSqlRecord>* alltracks = PL->getAllTracks();
  //emit(playthis(alltracks));
    if(libcont!=NULL)
        libcont->pushSmartPlaylist(PL->getPlaylistName(), PL->getFilter());
}

void PlaylistMenu::SmartRename()
{
  PLRenameDialog diag(PL, util);
}

void PlaylistMenu::SmartEdit()
{
  PLSmartEdit diag(PL, util);
}

void PlaylistMenu::SmartDelete()
{
  PL->deletePlaylist();
}


void PlaylistMenu::NormalPlay()
{
  emit playplaylist(PL->getPlaylistName());
}

void PlaylistMenu::NormalView()
{
//  QList<QSqlRecord>* alltracks = PL->getAllTracks();
//  emit(playthis(alltracks));
    libcont->pushNormalPlaylist(PL->getPlaylistName());
}

void PlaylistMenu::NormalRename()
{
  PLRenameDialog diag(PL, util);
}

void PlaylistMenu::NormalDelete()
{
  PL->deletePlaylist();
}

void PlaylistMenu::NewPlay()
{
  PLNormalNew diag(util, db);
}

void PlaylistMenu::NewSmartPlay()
{
  PLSmartNew diag(util, db);
}


