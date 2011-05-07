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
  setup2=0;
  addto = new AddTo(db);
  addto->setTitle("Add To...");
  menuitems[0] = this->addAction("Delete from my Library");
  menuitems[1] = this->addMenu(addto);

  QObject::connect(menuitems[0], SIGNAL(triggered()), this, SLOT(Delete()));
}

void TrackContext::trackRightClicked(QList<QString> ids, QList<QString> uniqueIDs, LibraryController* passlib)
{
  CurrentIDs = ids;
  CurrentUniqueIDs = uniqueIDs;
  addto->update(ids, uniqueIDs);
  if(setup == 0)
  {
    LibCont = passlib;
    setup = 1;
  }

  if(passlib->getCurrentViewType() == 1)
  {
    if(setup2==0)
    {
      menuitems[2] = this->addAction("Delete from This Playlist");
      QObject::connect(menuitems[2], SIGNAL(triggered()), this, SLOT(DeleteFromPlaylist()));
      setup2 = 1;
    }
    currentplaylistname = passlib->getCurrentPlaylistName();
  }
  else if(setup2 == 1)
  {
      setup2 = 0;
      this->removeAction(menuitems[2]);
  }
  this->exec(QCursor::pos());
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


