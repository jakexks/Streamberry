#ifndef PLAYLISTSCONTEXT_H
#define PLAYLISTSCONTEXT_H
#include <QtGui>

class QMenu;
class QSqlRecord;
class Playlist;
class LibraryController;
class Utilities;
class Database;


class PlaylistMenu : public QMenu
{
  Q_OBJECT

public:
  //PlaylistMenu(bool smart, Database& datab);
  PlaylistMenu(bool smart, Utilities* util, Database& datab);
  void playlistrightclicked(Playlist* pass, LibraryController* passlib);

signals:
  void playthis(QList<QSqlRecord>*);
  void playplaylist(QString);

 public slots:
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
  //QString currentplaylist;
  Database& db;
  bool currentsmart;
  int setup;
  Playlist* PL;
  LibraryController* libcont;
  Utilities* util;

};

#endif // PLAYLISTSCONTEXT_H
