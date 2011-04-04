#ifndef PLAYLISTDIALOGUE_H
#define PLAYLISTDIALOGUE_H
#include <QDialog>
#include <QObject>
#include <QWidget>
class Utilities;
class Playlist;
class QLineEdit;


class PLRenameDialog : public QDialog
{
   Q_OBJECT
public:
    PLRenameDialog(Playlist* playlist, Utilities* util);
private slots:
    void btnClicked();
private:
    Playlist* currentplaylist;
    QLineEdit* textbox;
    QDialog* dialog;

};

#endif // PLAYLISTDIALOGUE_H
