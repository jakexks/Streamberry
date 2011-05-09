#ifndef PLSMARTEDIT_H
#define PLSMARTEDIT_H
#include <QDialog>
#include <QObject>
#include <QWidget>
class Utilities;
class Playlist;
class QLineEdit;


//This class handles a dialog box used to create edit filter playlists.
class PLSmartEdit : public QDialog
{
   Q_OBJECT
public:
    //Instantiates the dialog box, passes it a database pointer and utilities so it can be styled
    PLSmartEdit(Playlist* playlist, Utilities* util);
private slots:
    //saves the changes to the filter, saves the playlist and closes the dialog.
    void btnClicked();
private:
    //holds a pointer to the current playlist
    Playlist* currentplaylist;
    //holds a pointer to the text box in the dialog box
    QLineEdit* textbox;
    //holds a pointer to the dialog box itself
    QDialog* dialog;

};

#endif // PLSMARTEDIT_H
