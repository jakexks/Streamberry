#ifndef TRACKCONTEXT_H
#define TRACKCONTEXT_H
#include <QtGui>

class QMenu;
class QSqlRecord;
class LibraryController;
class Database;

class TrackContext : QMenu
{
  Q_OBJECT

public:
  TrackContext();
  void trackRightClicked(QString file, QString uniqueID, LibraryController* passlib, Database* datab);

signals:
  void playthis(QList<QSqlRecord>*);

 public slots:
  void Play();
  void Delete();
  void Hide();

private:
  int setup;
  QString CurrentID;
  QString CurrentUniqueID;
  LibraryController* LibCont;
  Database* db;

};

#endif
