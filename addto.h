#ifndef ADDTO_H
#define ADDTO_H
#include <QtGui>

class QMenu;
class QSqlRecord;
class Database;

class AddTo : public QMenu
{
  Q_OBJECT

public:
  AddTo(Database* datab);
  void update(QList<QString> ids, QList<QString> uniqueIDs);

public slots:

  void Clicked();

private:
  int setup;
  QList<QString> CurrentIDs;
  QList<QString> CurrentUniqueIDs;


  Database* db;
};

#endif // ADDTO_H
