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
      QString temp2 = result2.value(1).toString();
      Trackpaths.append(temp2);
      TrackIDs.append(temp);
    }
  }
}

void Playlist::SavePlaylist()
{
  if(name != "")
  {
    db.PlaylistSave(name,smart,filter);

    db.PlaylistAddTracks(Trackpaths, TrackIDs, name);
  }
}

void Playlist::addTrack(QString ID, QString UniqueID)
{
  Trackpaths.append(ID);
  TrackIDs.append(UniqueID);
}

void Playlist::removeTrack(QString id, QString UniqueID)
{
  int index = Trackpaths.indexOf(id);
  removeTrack(index);
}

void Playlist::removeTrack(int index)
{
  TrackIDs.removeAt(index);
  Trackpaths.removeAt(index);
}

QList<QSqlRecord>* Playlist::getAllTracks()
{
  if(smart == 0) {
    return db.getTracks(Trackpaths, TrackIDs);
  }
  else
  {
    QList<QString> fields;
    QList<QString> order;
    fields.append("Album");
    order.append("DESC");
    QList<QSqlRecord>* result = db.searchDb(0, "", filter, fields, order, 0);
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

int Playlist::getPlaylistType()
{
  return smart;
}

int Playlist::getPlaylistLength()
{
  return Trackpaths.size();
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
