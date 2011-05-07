#ifndef PLNORMALNEW_H
#define PLNORMALNEW_H
#include <QDialog>
#include <QObject>
#include <QWidget>
class Utilities;
class Playlist;
class QLineEdit;
class Database;


//This class handles a dialog box used to create new normal playlists.
class PLNormalNew : public QDialog
{
  Q_OBJECT
public:
  //Instantiates the dialog box, passes it a database pointer and utilities so it can be styled
  PLNormalNew(Utilities* util, Database& datab);
private slots:
  //When the button is clicked a new playlist is created using the name from the first textbox
  void btnClicked();
private:
  //This holds a pointer to the current playlist
  Playlist* currentplaylist;
  //This holds a pointer to the textbox in the dialog box
  QLineEdit* textbox;
  //pointer to the dialog widget
  QDialog* dialog;
  //Holds a pointer to the database
  Database& db;

};

#endif // PLNORMALNEW_H
