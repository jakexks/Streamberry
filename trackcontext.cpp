#include "trackcontext.h"
#include <QMenu>
#include "utilities.h"
#include "librarycontroller.h"
#include <QtGui>
#include "database.h"
#include "addto.h"
#include "playlist.h"

TrackContext::TrackContext(Database* datab): db(datab)
{
  setup = 0;

  addto = new AddTo(db);
  addto->setTitle("Add To...");
  menuitems[0] = this->addAction("Delete from my Library");
  menuitems[1] = this->addAction("Toggle Hidden from Other Users");
  menuitems[2] = this->addMenu(addto);


  QObject::connect(menuitems[0], SIGNAL(triggered()), this, SLOT(Delete()));
  QObject::connect(menuitems[1], SIGNAL(triggered()), this, SLOT(Hide()));
}

void TrackContext::trackRightClicked(QList<QString> ids, QList<QString> uniqueIDs, LibraryController* passlib)
{
 // qDebug() << ids;
  CurrentIDs = ids;
  CurrentUniqueIDs = uniqueIDs;
  addto->update(ids, uniqueIDs);

  if(passlib->getCurrentViewType() == 1)
  {
    menuitems[3] = this->addAction("Delete from This Playlist");
    QObject::connect(menuitems[3], SIGNAL(triggered()), this, SLOT(DeleteFromPlaylist()));
    currentplaylistname = passlib->getCurrentPlaylistName();
  }

  if(setup == 0)
  {
    LibCont = passlib;
    //QObject::connect(this, SIGNAL(playthis(QList<QSqlRecord>*)), LibCont, SLOT(displaythis(QList<QSqlRecord>*)));
    setup = 1;
  }
  this->exec(QCursor::pos());
  this->removeAction(menuitems[3]);

}


void TrackContext::DeleteFromPlaylist()
{
  Playlist pl(*db, currentplaylistname);
  int i=0;
  for(i=0; i<CurrentIDs.size(); i++)
  {
    pl.removeTrack(CurrentIDs.at(i), CurrentUniqueIDs.at(i));
  }
  pl.SavePlaylist();
}

void TrackContext::Delete()
{
  int i=0;
  for(i=0; i<CurrentIDs.size(); i++)
  {
   db->deleteFile(CurrentIDs.at(i), CurrentUniqueIDs.at(i));
  }
}

void TrackContext::Hide()
{
  int i =0;
  for(i=0; i<CurrentIDs.size(); i++)
  {
    db->togglehidden(CurrentIDs.at(i), CurrentUniqueIDs.at(i));
  }
}

