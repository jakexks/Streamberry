#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "database.h"
#include <QtDebug>
#include <QString>
#include "sbexception.h"
#include "crossplatform.h"

//This class handles Playlists within Streamberry. Each currently open playlist must be represented by an instance of this class.
//It holds a playlist name, the number of tracks in the playlist, the type of playlist and a filter if the playlist is smart.

class Playlist
{
public:
  //This creates a new playlist, no fields are set. So this must be filled in using the setters before saving
  Playlist(Database &datab);
  //Creates a playlist object and fills it's private variables with info relating to the named playlist
  Playlist(Database &datab, QString playlistName);

  //Saves the playlist and all it's info back into the database
  void SavePlaylist();
  //Deletes the playlist from the database
  void deletePlaylist();

  //Adds a track to the playlist. Takes an id and uniqueID to identify the track.
  void addTrack(QString ID, QString UniqueID);
  //Removes a track from the playlist. Takes an id and uniqueID to identify the track.
  void removeTrack(QString ID, QString UniqueID);
  //Removes a track from the playlist, takes the index of the track in the playlist to identify the track.
  void removeTrack(int index) ;

  //Returns a QList of QSqlRecords containing all the tracks in this playlist. This contains all the needed data and should be
  //used for display in the main window.
  QList<QSqlRecord>* getAllTracks();
  //The next few methods are obvious in their function. Playlist length is the number of tracks in the playlist
  QString getPlaylistName();
  int getPlaylistType();
  int getPlaylistLength();
  QString getFilter();

  //The next few methods are obvious in their function.
  void setPlaylistName(QString newname);
  //Takes "smart" as a parameter if the playlist is a smart playlist
  void setPlaylistType(int type);
  void setFilter(QString newfilter);

private:
  Database& db;
  QString name;
  int smart;
  QString filter;
  QList<QString> Trackpaths;
  QList<QString> TrackIDs;
};

#endif
