#ifndef TRACKCONTEXT_H
#define TRACKCONTEXT_H
#include <QtGui>


class AddTo;
class QMenu;
class QSqlRecord;
class LibraryController;
class Database;

//This class handles context menu that appears when you right click on a track in the main library view.
//Allowing the deletion of tracks from the library and adding to playlists (via AddTo). When viewing a playlist
//you can also delete tracks from that playlist.
class TrackContext : QMenu
{
  Q_OBJECT

public:
  //Instantiates a TrackContext and connects it to the database
  TrackContext(Database* datab);
  //When a track is right clicked the class is updated to know which tracks have currently been selected.
  //Therefore when an action is taken it can operate on the correct tracks.
  void trackRightClicked(QList<QString> ids, QList<QString> uniqueIDs, LibraryController* passlib);

signals:
  void playthis(QList<QSqlRecord>*);
 public slots:
  void DeleteFromPlaylist();
  void Delete();

private:
  //When 1 the menu has been setup and the QObject connections made
  int setup;
  //Stores the name of the current playlist (if there is one) to allow tracks to be deleted from the playlist
  QString currentplaylistname;
  //Holds a qlist of all the filepaths of the tracks selected in the main window
  QList<QString> CurrentIDs;
  //Holds a qlist of all the uniqueIDs of the tracks selected in the main window
  QList<QString> CurrentUniqueIDs;
  //pointer to the Library controller class
  LibraryController* LibCont;
  //pointer to the database class
  Database* db;
  //pointer to the addTo class
  AddTo* addto;
  //when Setup2 is 1 then the menu has been setup for use in the playlist view.
  int setup2;
  //stores pointers to the menu functions
  QAction* menuitems[3];

};

#endif
