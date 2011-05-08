#ifndef MAXPLAYLISTS_H
#define MAXPLAYLISTS_H
#include <QDialog>
#include <QObject>
#include <QWidget>

class Utilities;
class Database;

//Controls a maxmimum number of playlists
class MaxPlaylists: public QDialog
{
   Q_OBJECT

public:
  MaxPlaylists(Utilities* util, Database& datab);

public slots:
  void btnClicked();

private:
  QDialog* dialog;
#endif // MAXPLAYLISTS_H
}  ;
