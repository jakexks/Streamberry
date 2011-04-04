#include "playlist.h"

Playlist::Playlist(Database &datab) : db(datab)
{
    smart = 0;
    filter = "";
}

Playlist::Playlist(Database &datab, QString playlistName) : db(datab)
{
  QSqlQuery result = db.GetPlaylistInfo(playlistName);
  result.first();
  name = result.value(0).toString();
  smart = result.value(1).toInt();
  filter = result.value(2).toString();
  QSqlQuery result2 = db.GetPlaylistTracks(playlistName);
  while ( result2.next() )
  {
    if (result2.isValid())
    {
      QString temp = result2.value(0).toString();
      temp += ":";
      temp += result2.value(1).toString();
      Tracks.append(temp);
    }
  }
}

void Playlist::SavePlaylist()
{
  db.PlaylistSave(name,smart,filter);
  db.PlaylistAddTracks(Tracks, name);
}

void Playlist::addTrack(int ID, QString UniqueID)
{
  QString newtrack;
  newtrack = QString::number(ID);
  newtrack += ":";
  newtrack += UniqueID;
  Tracks.append(newtrack);
}

void Playlist::removeTrack(int ID, QString UniqueID)
{
  QString track;
  track = QString::number(ID);
  track += ":";
  track += UniqueID;
  int index = Tracks.indexOf(track);
  this->removeTrack(index);
}

void Playlist::removeTrack(int index)
{
  Tracks.removeAt(index);
}

QList<QSqlRecord>* Playlist::getAllTracks()
{
  if(smart == 0)
    return db.getTracks(Tracks);
  else
  {
      //Only library controller should ever call this - Robbie
//    QList<QString> fields;
//    QList<QString> order;
//    fields.append("Album");
//    order.append("DESC");
//    QList<QSqlRecord>* result = db.searchDb(0, filter, fields, order);
//    return result;
  }
}

void Playlist::deletePlaylist()
{
  db.removePlaylist(name);
}

QString Playlist::getPlaylistName()
{
  return name;
}

void Playlist::setPlaylistName(QString newname)
{
   name = newname;
}

int Playlist::getPlaylistType()
{
   return smart;
}

int Playlist::getPlaylistLength()
{
  return Tracks.size();
}

void Playlist::setPlaylistType(int type)
{
  smart = type;
}

void Playlist::setFilter(QString newfilter)
{
  filter = newfilter;
}

QString Playlist::getFilter()
{
  return filter;
}
