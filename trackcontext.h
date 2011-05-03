#ifndef TRACKCONTEXT_H
#define TRACKCONTEXT_H
#include <QtGui>

class AddTo;
class QMenu;
class QSqlRecord;
class LibraryController;
class Database;

class TrackContext : QMenu
{
  Q_OBJECT

public:
  TrackContext(Database* datab);
  void trackRightClicked(QList<QString> ids, QList<QString> uniqueIDs, LibraryController* passlib);

signals:
  void playthis(QList<QSqlRecord>*);

 public slots:
  void DeleteFromPlaylist();
  void Delete();
  void Hide();

private:
  int setup;
  QString currentplaylistname;
  QList<QString> CurrentIDs;
  QList<QString> CurrentUniqueIDs;
  LibraryController* LibCont;
  Database* db;
  AddTo* addto;
  int setup2;
  QAction* menuitems[3];

};

#endif
