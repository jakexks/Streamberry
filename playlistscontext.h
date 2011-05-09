#ifndef PLAYLISTSCONTEXT_H
#define PLAYLISTSCONTEXT_H
#include <QtGui>

class QMenu;
class QSqlRecord;
class Playlist;
class LibraryController;
class Utilities;
class Database;

//Playlist context menu creates and handles the context menu that appears when you right-click a playlist. This
//Menu appears differently depending on whether a smart playlist or a normal playlist is right clicked
class PlaylistMenu : public QMenu
{
  Q_OBJECT


public:
  //instantiates the context menu and initialises it's connection with the database
  PlaylistMenu(bool smart, Utilities* util, Database& datab);
  //called when a playlist is right clicked, sets up the Playlist pointer within the class.
  void playlistrightclicked(Playlist* pass, LibraryController* passlib);

signals:
  //When play is clicked then this is sent to LibraryController. Depreciated.
  void playthis(QList<QSqlRecord>*);
  //Sends a signal to the Libcontroller with the name of the playlist to be played
  void playplaylist(QString);
  //Sends a signal to the Libcontroller with the filter of the filter playlist to be played
  void playsmartplaylist(QString);


public slots:
  //The slots below are obvious in their function. A slot is needed for every possible action that the playlist
  //context menu can output.
  void SmartPlay();
  void SmartView();
  void SmartRename();
  void SmartEdit();
  void SmartDelete();
  void NormalPlay();
  void NormalView();
  void NormalRename();
  void NormalDelete();
  void NewPlay();
  void NewSmartPlay();


private:
  ///Holds a pointer to the database class
  Database& db;
  //if set to one then the currently right clicked on playlist is a fitler playlist and the menu should
  //be drawn appropriately.
  bool currentsmart;
  //If 1 then the menu has been set up already, this helps prevent a double set-up
  int setup;
  //holds a pointer to the currenty right-clicked playlist
  Playlist* PL;
  //holds a pointer to the Library Controller
  LibraryController* libcont;
  //Holds a pointer to the utils class
  Utilities* util;

};

#endif // PLAYLISTSCONTEXT_H
