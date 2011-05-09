#ifndef ADDTO_H
#define ADDTO_H
#include <QtGui>

class QMenu;
class QSqlRecord;
class Database;

//This class handles the context menu attached to TrackContext which displays all explicit playlists. It allows adding of
//tracks to playlists either individually or in groups
class AddTo : public QMenu
{
  Q_OBJECT

public:
    //Creates a new AddTo menu and initialises it's connection with the database
  AddTo(Database* datab);

  //When a track is right clicked the AddTo menu must be updated to ensure it will contain all the
  //needed playlists, including any that have been made since the AddTo's creation. This function also sets up
  //the AddTo menu with knowledge of which tracks are currently selected, so they can be added to a playlist.
  void update(QList<QString> ids, QList<QString> uniqueIDs);

public slots:
  //When a playlist title is clicked on this slot is called and the selected tracks are added to the clicked playlist
  void Clicked();

private:
  //Setup is initialially set to 0, but once AddTo setup has been completed it is set to one
  //this is to prevent 'setting up' functions from running more than once
  int setup;

  //Holds a list of all the filepaths of the currently selected tracks at the time of right-clicking
  QList<QString> CurrentIDs;

  //Holds a list of all the Unique IDs of the currently selected tracks at the time of right-clicking
  QList<QString> CurrentUniqueIDs;

  //This holds a pointer to the Database class so that pointers to playlists can be created.
  Database* db;
};

#endif // ADDTO_H
