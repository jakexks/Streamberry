#ifndef PLAYLISTDIALOGUE_H
#define PLAYLISTDIALOGUE_H
#include <QDialog>
#include <QObject>
#include <QWidget>
class Utilities;
class Playlist;
class QLineEdit;

//This class handles a dialog box used to rename playlists.
class PLRenameDialog : public QDialog
{
   Q_OBJECT
public:
  //Instantiates the dialog box, passes it a playlist pointer (for the currently selected playlist
  //and utilities so it can be styled)
    PLRenameDialog(Playlist* playlist, Utilities* util);
    ~PLRenameDialog();

private slots:
    //This is called when the accept button is pressed. It renames the playlist using the Playlist object
    //The playlist is then saved and the box closed.
    void btnClicked();
private:
    //Holds a pointer to the current playlist
    Playlist* currentplaylist;
    //This holds a pointer to the textbox in the dialog box
    QLineEdit* textbox;
    //pointer to the dialog widget
    QDialog* dialog;

};

#endif // PLAYLISTDIALOGUE_H
