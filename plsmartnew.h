#ifndef PLSMARTNEW_H
#define PLSMARTNEW_H
#include <QDialog>
#include <QObject>
#include <QWidget>
class Utilities;
class Playlist;
class QLineEdit;
class Database;

//This class handles a dialog box used to create new filter playlists.
class PLSmartNew : public QDialog
{
   Q_OBJECT
public:
  //Instantiates the dialog box, passes it a database pointer and utilities so it can be styled
    PLSmartNew(Utilities* util, Database& datab);
private slots:
    //When the button is clicked a new playlist is created using the name
    //from the first textbox and the filter terms from the second textbox.
    void btnClicked();
private:
    //This holds a pointer to the first textbox in the dialog box
    QLineEdit* textbox1;
    //This holds a pointer to the second textbox in the dialog box
    QLineEdit* textbox2;
    //pointer to the dialog widget
    QDialog* dialog;
    //Holds a pointer to the database
    Database& db;

};

#endif // PLSMARTNEW_H
