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
    void btnClicked();
private:
    Playlist* currentplaylist;
    QLineEdit* textbox;
    QDialog* dialog;

};

#endif // PLSMARTEDIT_H
