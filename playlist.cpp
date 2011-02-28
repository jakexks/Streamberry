#include "playlist.h"

Playlist::Playlist(Database &datab) : db(datab)
{
    db = datab;
    smart = false;
}

Playlist::Playlist(Database &datab, QString playlistName) : db(datab)
{
  db = datab;
  QSqlQuery result = db.GetPlaylistInfo(playlistName);
  result.first();
  name = result.value(0).toString();
  smart = result.value(1).toBool();
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
  db.PlaylistSave(name,(smart == true ? 1 : 0),filter);
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

QList<QSqlRecord> Playlist::getAllTracks()
{
  if(smart == false)
    return db.getTracks(Tracks);
  else
  {
    QList<QString> fields;
    QList<QString> order;
    fields.append("Album");
    order.append("DESC");
    QList<QSqlRecord> result = *db.searchDb(0, filter, fields, order);
    return result;
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

bool Playlist::getPlaylistType()
{
   return smart;
}

int Playlist::getPlaylistLength()
{
  return Tracks.size();
}

void Playlist::setPlaylistType(QString type)
{
  if(type == "smart")
    smart = true;
  else
    smart = false;
}

void Playlist::setFilter(QString newfilter)
{
  smart = true;
  filter = newfilter;
}

QString Playlist::getFilter()
{
  return filter;
}
