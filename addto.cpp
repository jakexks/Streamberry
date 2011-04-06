#include "addto.h"
#include <QMenu>
#include "utilities.h"
#include "playlist.h"
#include <QtGui>
#include "database.h"
#include <QAction>

AddTo::AddTo(Database* datab) : db(datab)
{
}

void AddTo::update(QList<QString> ids, QList<QString> uniqueIDs)
{

  CurrentIDs = ids;
  CurrentUniqueIDs = uniqueIDs;

  QList<QSqlRecord>* playlists = db->getAllPlaylists();
  this->clear();
  int playlistnumber = playlists->size();
  QAction* menuitems[playlistnumber];
  for(int i=0; i<playlistnumber; i++)
  {
    if(playlists->at(i).field(1).value().toString() == "0")
    {
      menuitems[i] = this->addAction(playlists->at(i).field(0).value().toString());
      menuitems[i]->setObjectName(playlists->at(i).field(0).value().toString());
      QObject::connect(menuitems[i], SIGNAL(triggered()), this, SLOT(Clicked()));
    }
  }

}

void AddTo::Clicked()
{
  QObject* temp = sender();
  QString playlistname = temp->objectName();
  Playlist pl(*db, playlistname);
  for(int i=0; i<CurrentIDs.size(); i++)
  {
    pl.addTrack(CurrentIDs.at(i), CurrentUniqueIDs.at(i));
  }
  pl.SavePlaylist();
}

