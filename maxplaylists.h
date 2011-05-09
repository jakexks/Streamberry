#ifndef MAXPLAYLISTS_H
#define MAXPLAYLISTS_H
#include <QDialog>
#include <QObject>
#include <QWidget>

class Utilities;
class Database;

//Displays a dialog box when the maximum number of playlists is reached
class MaxPlaylists: public QDialog
{
   Q_OBJECT

public:
  //instantiates the dialog box
  MaxPlaylists(Utilities* util, Database& datab);

public slots:
  //closes the dialog box
  void btnClicked();

private:
  QDialog* dialog;
#endif // MAXPLAYLISTS_H
}  ;
