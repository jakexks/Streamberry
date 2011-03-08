#ifndef PLAYLISTSCONTEXT_H
#define PLAYLISTSCONTEXT_H
#include <QtGui>

class QMenu;
class QSqlRecord;
class Playlist;
class LibraryController;


class PlaylistMenu : QMenu
{
  Q_OBJECT

public:
  //PlaylistMenu(bool smart, Database& datab);
  PlaylistMenu(bool smart);
  void playlistrightclicked(Playlist* pass, LibraryController* passlib);

signals:
  void playthis(QList<QSqlRecord>*);

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

private:
  //QString currentplaylist;
  bool currentsmart;
  int setup;
  Playlist* PL;
  LibraryController* LibCont;

};

#endif // PLAYLISTSCONTEXT_H
